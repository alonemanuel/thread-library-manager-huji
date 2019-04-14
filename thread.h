//
// Created by kalir on 27/03/2019.
//

#ifndef EX2_THREAD_H

#include "uthreads.h"
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <memory>

#define EX2_THREAD_H
#define READY 0
#define BLOCKED 1
#define RUNNING 2

typedef unsigned long address_t;
#define JB_SP 6
#define JB_PC 7


using std::shared_ptr;

class Thread
{

private:
	static int num_of_threads;
protected:
	int _state;
	int _stack_size;
	int _id;
	int _quantums;

public:

	void (*func)(void);

	char *stack;
	sigjmp_buf env[1];
	address_t sp, pc;


	Thread(void (*f)(void) = nullptr, int id=0);

	int get_id() const
	{ return _id; };

	int get_state()
	{ return _state; };

	void set_state(int state)
	{ _state = state; };

	int get_quantums()
	{
		return _quantums;
	}

	void increase_quantums()
	{
		_quantums++;
	}

	bool operator==(const Thread &other) const;
};

#endif //EX2_THREAD_H
