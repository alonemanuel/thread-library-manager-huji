
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

/**
 * @brief exiting due to error function
 * @param code error code
 * @param text explanatory text for the error
 */
int exit_with_err(int code, string text)
{
	string prefix;
	int ret_val;
	switch (code)
	{
		case SYS_ERR_CODE:
			prefix = "system error: ";
			ret_val = 1;
		case THREAD_ERR_CODE:
			prefix = "thread error: ";
			ret_val = -1; // TODO: should return appropriate return value;
	}
	cerr << prefix << text << endl;
	exit(ret_val);    // TODO we need to return on failures, but exit makes it irrelevant

}


bool does_exist(std::list<shared_ptr<Thread>> lst, int tid)
{
	for (std::list<shared_ptr<Thread>>::iterator it = lst.begin(); it != lst.end(); ++it)
	{
		if ((*it)->get_id() == tid)
		{
			return true;
		}
	}
	return false;
}


timeval calc_wake_up_timeval(int usecs_to_sleep)
{

	timeval now, time_to_sleep, wake_up_timeval;
	gettimeofday(&now, nullptr);
	time_to_sleep.tv_sec = usecs_to_sleep / 1000000;
	time_to_sleep.tv_usec = usecs_to_sleep % 1000000;
	timeradd(&now, &time_to_sleep, &wake_up_timeval);
	return wake_up_timeval;
}

/**
 * @brief make the front of the ready threads list the current running thread.
 */
int ready_to_running()
{
	// push the current running thread to the back of the ready threads
	ready_threads.push_back(running_thread);
	// pop the topmost ready thread to be the running thread
	running_thread = ready_threads.front();
	running_thread->increase_quantums();
	ready_threads.pop_front();
	// TODO: maybe move this to the general case of a thread starting a run
	// increase thread's quantum counter
	running_thread->increase_quantums();
	total_quantums++;

	// start timer for the running thread

	if (setitimer(ITIMER_VIRTUAL, &quantum_timer, NULL))
	{
		// TODO: print error
		return -1;    // TODO: bubble up error
	}
	// jump to the running thread's last state
	siglongjmp(running_thread->env[0], 1);
}

// Handlers //
void quantum_handler(int sig)
{
	// assuming sig = SIGVTALRM
	sigsetjmp(running_thread->env[0], 1);
	ready_to_running();
}


void sleep_handler(int sig)
{
	// assuming sig = SIGALRM
	uthread_resume(sleeping_threads.peek()->id);
	sleeping_threads.pop();
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
	// 1 because of the main thread
	total_quantums = 1;

	// quantum_usecs cannot be negative
	if (quantum_usecs < 0)
	{
		exit_with_err(THREAD_ERR_CODE, "quantum usecs should be non-negative.");
	}

	// init quantum timer
	quantum_timer.it_value.tv_sec = 0;
	quantum_timer.it_value.tv_usec = quantum_usecs;
	quantum_sa.sa_handler = &quantum_handler;
	if (sigaction(SIGVTALRM, &quantum_sa, NULL) < 0)
	{
		exit_with_err(SYS_ERR_CODE, "timer initialization failed.");
	}

	// init sleep timer
	sleep_timer.it_value.tv_sec = 0;
	sleep_timer.it_value.tv_usec = 0;
	sleep_sa.sa_handler = &sleep_handler;
	if (sigaction(SIGALRM, &sleep_sa, NULL) < 0)
	{
		exit_with_err(SYS_ERR_CODE, "timer initialization failed.");
	}

	// set timer
	if (setitimer(ITIMER_VIRTUAL, &quantum_timer, NULL))
	{
		// TODO: print error
		return -1;    // TODO: bubble up error
	}

	// create main thread
	shared_ptr<Thread> new_thread = std::make_shared<Thread>(Thread());
	threads[new_thread->get_id()] = new_thread;
	running_thread = new_thread;
	running_thread->increase_quantums();

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
	if (ready_threads.size() == MAX_THREAD_NUM)
	{
		return -1; // TODO: error msg? constant?
	}
	shared_ptr<Thread> new_thread = std::make_shared<Thread>(Thread(f));
	threads[new_thread->get_id()] = new_thread;
	ready_threads.push_back(new_thread);
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
{ //TODO: consider an error
	if (threads.find(tid) == threads.end())
	{
		return -1;
	}
	if (running_thread->get_id() == tid)
	{
		ready_to_running();
	}
	else if (does_exist(ready_threads, tid))
	{
		ready_threads.remove(threads[tid]);
	}
	threads.erase(tid);
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
	// don't allow blocking of the main thread (or a non existing one)
	if (tid == 0 || threads.find(tid) == threads.end())
	{
		return -1;
	}
	// if thread is the running thread, run the next ready thread
	if (threads[tid] == running_thread)
	{
		ready_to_running();
	}
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
	if (threads.find(tid) == threads.end())
	{
		return -1;
	}
	shared_ptr<Thread> curr_thread = threads[tid];
	// if thread to resume is not running or already ready
	if (does_exist(ready_threads, tid) && (running_thread->get_id() != tid))
	{
		ready_threads.push_back(curr_thread);
	}
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
int uthread_sleep(unsigned int usec)
{
	// don't allow main thread sleeping
	if (running_thread->get_id() == 0)
	{
		return -1;
	}
	// update sleep_timer values
	sleep_timer.it_value.tv_usec = usec;
	sleeping_threads.add(running_thread->get_id(), calc_wake_up_timeval(usec));
	if (setitimer(ITIMER_REAL, &sleep_timer, NULL))
	{
		// TODO: print error
		return -1;    // TODO: bubble up error
	}
	ready_to_running();
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
