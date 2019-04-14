//
// Created by kalir on 27/03/2019.
//

#include <iostream>
#include "uthreads.h"
#include "thread.h"
#include <memory>

using std::cout,
std::endl;

int Thread::num_of_threads = 0;

/* A translation is required when using an address of a variable.
   Use this as a black box in your code. */
address_t translate_address(address_t addr)
{
	address_t ret;
	asm volatile("xor    %%fs:0x30,%0\n"
				 "rol    $0x11,%0\n"
	: "=g" (ret)
	: "0" (addr));
	return ret;
}

/**
 * @brief Constructor of a thread object
 * @param f thread function address
 */
Thread::Thread(void (*f)(void), int id) : _id(id), _state(READY), _stack_size(STACK_SIZE), _quantums(0), func(f)
{
	stack = new char[STACK_SIZE];
//	if (num_of_threads)
//	{
//	cout << "Creating thread!" << endl;
	sp = (address_t) stack + STACK_SIZE - sizeof(address_t);
	pc = (address_t) f;
	sigsetjmp(env[0], 1);
	(env[0]->__jmpbuf)[JB_SP] = translate_address(sp);
	(env[0]->__jmpbuf)[JB_PC] = translate_address(pc);
	sigemptyset(&env[0]->__saved_mask);
//	}
	num_of_threads++;
}


bool Thread::operator==(const Thread &other) const
{
	return _id == other.get_id();
}

