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

#define EX2_THREAD_H
#define READY 0
#define BLOCKED 1
#define RUNNING 2

typedef unsigned long address_t;
#define JB_SP 6
#define JB_PC 7

class Thread
{

private:
	static int num_of_threads;
	address_t sp, pc;
	char stack[STACK_SIZE];


protected:
	int _state;
	int _stack_size;
	int _id;
	int _quantums;

	void (*func)(void);

public:
	sigjmp_buf env[1];


	Thread(void (*f)(void) = nullptr) : _state(READY), _stack_size(STACK_SIZE), _id
			(num_of_threads), _quantums(0), func(f)
	{}

	int get_id()
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
};

#endif //EX2_THREAD_H
