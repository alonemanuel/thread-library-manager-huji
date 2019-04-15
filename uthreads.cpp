
#include "uthreads.h"
#include "thread.h"
#include <list>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <memory>
#include "sleeping_threads_list.h"

// Constants //
#define SYS_ERR_CODE 0
#define THREAD_ERR_CODE 1

// Using //

using std::cout,
std::endl;

using std::shared_ptr;


// Static Variables //
int total_quantums;

sigjmp_buf env[2];
int current_thread;

sigset_t sigs_to_block;

/**
 * @brief map of all existing threads, with their tid as key.
 */
std::unordered_map<int, shared_ptr<Thread>> threads;
/**
 * @brief list of all ready threads.
 */
std::list<shared_ptr<Thread>> ready_threads;
/**
 * @brief the current running thread.
 */
shared_ptr<Thread> running_thread;
/**
 * @brief list of all current sleeping threads (id's).
 */
SleepingThreadsList sleeping_threads;
/**
 * @brief timers.
 */

struct itimerval quantum_timer, sleep_timer;
/**
 * @brief sigactions.
 */
struct sigaction quantum_sa, sleep_sa;


// Helper Functions //

void block_signals()
{
	sigprocmask(SIG_SETMASK, &sigs_to_block, NULL);

}

void unblock_signals()
{
	sigprocmask(SIG_UNBLOCK, &sigs_to_block, NULL);

}

int get_min_id()
{
	block_signals();

	for (int i = 0; i < threads.size(); ++i)
	{
		if (threads.find(i) == threads.end())
		{
			unblock_signals();
			return i;
		}
	}
	unblock_signals();
	return threads.size();

}

/**
 * @brief exiting due to error function
 * @param code error code
 * @param text explanatory text for the error
 */
int print_err(int code, string text)
{
	block_signals();
	string prefix;
	switch (code)
	{
		case SYS_ERR_CODE:
			prefix = "system error: ";
			break;
		case THREAD_ERR_CODE:
			prefix = "thread library error: ";
			break;
	}
	cout << prefix << text << endl;
	if (code == SYS_ERR_CODE)
	{
		exit(1);    // TODO we need to return on failures, but exit makes it irrelevant
	}
	else
	{
		unblock_signals();
		return -1;
	}

}


bool does_exist(std::list<shared_ptr<Thread>> lst, int tid)
{
	block_signals();
	for (std::list<shared_ptr<Thread>>::iterator it = lst.begin(); it != lst.end(); ++it)
	{
		if ((*it)->get_id() == tid)
		{
			unblock_signals();
			return true;
		}
	}
	unblock_signals();
	return false;
}

void init_sigs_to_block()
{
	block_signals();
	sigemptyset(&sigs_to_block);
	sigaddset(&sigs_to_block, SIGALRM);
	sigaddset(&sigs_to_block, SIGVTALRM);
	unblock_signals();
}


timeval calc_wake_up_timeval(int usecs_to_sleep)
{
	block_signals();
	timeval now, time_to_sleep, wake_up_timeval;
	gettimeofday(&now, nullptr);
	time_to_sleep.tv_sec = usecs_to_sleep / 1000000;
	time_to_sleep.tv_usec = usecs_to_sleep % 1000000;
	timeradd(&now, &time_to_sleep, &wake_up_timeval);
	unblock_signals();
	return wake_up_timeval;
}

/**
 * @brief make the front of the ready threads list the current running thread.
 */
void ready_to_running(bool is_blocking = 0)
{
//	cout<<"curr running tid is " <<running_thread->get_id()<<endl;
	block_signals();
	int ret_val = sigsetjmp(running_thread->env[0], 1);
//	printf("SWITCH: ret_val=%d\n", ret_val);
	if (ret_val == 1)
	{
		unblock_signals();
		return;
	}
	if (!is_blocking)
	{
		// push the current running thread to the back of the ready threads
		ready_threads.push_back(running_thread);
	}
	// pop the topmost ready thread to be the running thread
	running_thread = ready_threads.front();

//	cout<<"after switch running tid is " <<running_thread->get_id()<<endl;
	ready_threads.pop_front();
	// increase thread's quantum counter
	running_thread->increase_quantums();
	// TODO: maybe move this to the general case of a thread starting a run
	total_quantums++;
	// jump to the running thread's last state
	if (setitimer(ITIMER_VIRTUAL, &quantum_timer, NULL))
	{
		unblock_signals();
		print_err(SYS_ERR_CODE, TIMER_SET_MSG);
	}
	unblock_signals();
	siglongjmp(running_thread->env[0], 1);
}


// Handlers //
void quantum_handler(int sig)
{

	block_signals();
	ready_to_running();
	unblock_signals();
}


void sleep_handler(int sig)
{
	block_signals();
	uthread_resume(sleeping_threads.peek()->id);
	sleeping_threads.pop();
	unblock_signals();
}


// API Functions //

/*
 * Description: This function initializes the thread library.
 * You may assume that this function is called before any other thread library
 * function, and that it is called exactly once. The input to the function is
 * the length of a quantum in micro-seconds. It is an error to call this
 * function with non-positive quantum_usecs.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_init(int quantum_usecs)
{
	block_signals();

	// 1 because of the main thread
	total_quantums = 1;

	// quantum_usecs cannot be negative
	if (quantum_usecs < 0)
	{
		print_err(THREAD_ERR_CODE, "quantum usecs should be non-negative.");
	}

	// init quantum timer
	quantum_timer.it_value.tv_sec = quantum_usecs / 1000000;
	quantum_timer.it_value.tv_usec = quantum_usecs % 1000000;
//	quantum_timer.it_interval.tv_sec = 0;
//	quantum_timer.it_interval.tv_sec =0;
	quantum_sa.sa_handler = &quantum_handler;
	if (sigaction(SIGVTALRM, &quantum_sa, NULL) < 0)
	{
		print_err(SYS_ERR_CODE, "timer initialization failed.");
	}

	// init sleep timer
	sleep_timer.it_value.tv_sec = 0;
	sleep_timer.it_value.tv_usec = 0;
//	sleep_timer.it_interval.tv_usec = 0;
//	sleep_timer.it_interval.tv_usec = 0;
	sleep_sa.sa_handler = &sleep_handler;
	if (sigaction(SIGALRM, &sleep_sa, NULL) < 0)
	{
		print_err(SYS_ERR_CODE, "timer initialization failed.");
	}

	// set timer
	if (setitimer(ITIMER_VIRTUAL, &quantum_timer, NULL))
	{
		print_err(SYS_ERR_CODE, "timer set failed.");
	}

	// create main thread
	shared_ptr<Thread> new_thread = std::make_shared<Thread>(Thread());
	threads[new_thread->get_id()] = new_thread;
	running_thread = new_thread;
	running_thread->increase_quantums();

	init_sigs_to_block();
	unblock_signals();
	return 0;
}


/*
 * Description: This function creates a new thread, whose entry point is the
 * function f with the signature void f(void). The thread is added to the end
 * of the READY threads list. The uthread_spawn function should fail if it
 * would cause the number of concurrent threads to exceed the limit
 * (MAX_THREAD_NUM). Each thread should be allocated with a stack of size
 * STACK_SIZE bytes.
 * Return value: On success, return the ID of the created thread.
 * On failure, return -1.
*/
int uthread_spawn(void (*f)(void))
{
	block_signals();
	if (threads.size() == MAX_THREAD_NUM)
	{
		return (print_err(THREAD_ERR_CODE, "max number of threads exceeded."));
	}

	shared_ptr<Thread> new_thread = std::make_shared<Thread>(Thread(f, get_min_id()));
	threads[new_thread->get_id()] = new_thread;
	ready_threads.push_back(new_thread);
	unblock_signals();
	return new_thread->get_id();
}


/*
 * Description: This function terminates the thread with ID tid and deletes
 * it from all relevant control structures. All the resources allocated by
 * the library for this thread should be released. If no thread with ID tid
 * exists it is considered an error. Terminating the main thread
 * (tid == 0) will result in the termination of the entire process using
 * exit(0) [after releasing the assigned library memory].
 * Return value: The function returns 0 if the thread was successfully
 * terminated and -1 otherwise. If a thread terminates itself or the main
 * thread is terminated, the function does not return.
*/
int uthread_terminate(int tid)
{ //TODO: consider an error and memory deallocation
	block_signals();
	if (threads.find(tid) == threads.end())
	{
		unblock_signals();
		return print_err(THREAD_ERR_CODE, "invalid thread id.");
	}
	if (tid == 0)
	{
		exit(0);
	}
	if (running_thread->get_id() == tid)
	{
		threads.erase(tid);
		sleeping_threads.remove(tid);
		ready_to_running(1);
	}
	else
	{
		if (does_exist(ready_threads, tid))
		{
			ready_threads.remove(threads[tid]);
			sleeping_threads.remove(tid);
		}


		threads.erase(tid);
		sleeping_threads.remove(tid);

	}
	unblock_signals();
}


/*
 * Description: This function blocks the thread with ID tid. The thread may
 * be resumed later using uthread_resume. If no thread with ID tid exists it
 * is considered as an error. In addition, it is an error to try blocking the
 * main thread (tid == 0). If a thread blocks itself, a scheduling decision
 * should be made. Blocking a thread in BLOCKED state has no
 * effect and is not considered an error.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_block(int tid)
{
	block_signals();
	shared_ptr<Thread> to_delete;
	// don't allow blocking of the main thread (or a non existing one)
	if (tid == 0 || threads.find(tid) == threads.end())
	{
		return print_err(THREAD_ERR_CODE, "unable to block non-existing/main thread");
	}




	// if thread is the running thread, run the next ready thread
	if (threads[tid] == running_thread)
	{
		ready_to_running(1);
	}
	// block thread (remove from ready)
	for (std::list<shared_ptr<Thread>>::iterator it = ready_threads.begin(); it != ready_threads.end(); ++it)
	{
		if ((*it)->get_id() == tid)
		{
			to_delete = *it;
		}
	}
	if (to_delete != nullptr)
	{
		ready_threads.remove(to_delete);
	}
	unblock_signals();
	return 0;
}


/*
 * Description: This function resumes a blocked thread with ID tid and moves
 * it to the READY state. Resuming a thread in a RUNNING or READY state
 * has no effect and is not considered as an error. If no thread with
 * ID tid exists it is considered an error.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_resume(int tid)
{
	block_signals();
	if (threads.find(tid) == threads.end())
	{
		return print_err(THREAD_ERR_CODE, "no such thread exsits.");
	}
	shared_ptr<Thread> curr_thread = threads[tid];
	// if thread to resume is not running or already ready
	if (does_exist(ready_threads, tid) || (running_thread->get_id() != tid))
	{
		ready_threads.push_back(curr_thread);
	}
	unblock_signals();
	return 0;
}


/*
 * Description: This function blocks the RUNNING thread for user specified micro-seconds (REAL
 * time).
 * It is considered an error if the main thread (tid==0) calls this function.
 * Immediately after the RUNNING thread transitions to the BLOCKED state a scheduling decision
 * should be made.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_sleep(int usec)
{
	block_signals();
	if(usec<0){
		unblock_signals();
		return print_err(SYS_ERR_CODE, "invalid time value");
	}
//	else if (usec == 0){
//		unblock_signals();
//		return 0;
//	}
	// don't allow main thread sleeping
	if (running_thread->get_id() == 0)
	{
		return print_err(THREAD_ERR_CODE, "main thread can't be put to sleep.");
	}
	// update sleep_timer values
	sleep_timer.it_value.tv_sec = usec / 1000000;
	sleep_timer.it_value.tv_usec = usec % 1000000;
	if (setitimer(ITIMER_REAL, &sleep_timer, NULL))
	{
		print_err(SYS_ERR_CODE, "setting timer failed.");
	}
	sleeping_threads.add(running_thread->get_id(), calc_wake_up_timeval(usec));
	ready_to_running(1);
	unblock_signals();
	return 0;
}


/*
 * Description: This function returns the thread ID of the calling thread.
 * Return value: The ID of the calling thread.
*/
int uthread_get_tid()
{
	return running_thread->get_id();
}

/*
 * Description: This function returns the total number of quantums since
 * the library was initialized, including the current quantum.
 * Right after the call to uthread_init, the value should be 1.
 * Each time a new quantum starts, regardless of the reason, this number
 * should be increased by 1.
 * Return value: The total number of quantums.
*/
int uthread_get_total_quantums()
{
	return total_quantums;
}

/*
 * Description: This function returns the number of quantums the thread with
 * ID tid was in RUNNING state. On the first time a thread runs, the function
 * should return 1. Every additional quantum that the thread starts should
 * increase this value by 1 (so if the thread with ID tid is in RUNNING state
 * when this function is called, include also the current quantum). If no
 * thread with ID tid exists it is considered an error.
 * Return value: On success, return the number of quantums of the thread with ID tid.
 * 			     On failure, return -1.
*/
int uthread_get_quantums(int tid)
{
	// check for non existing tid
	if (threads.find(tid) == threads.end())
	{
		return -1;
	}
	return threads[tid]->get_quantums();
}
