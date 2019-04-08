//
// Created by kalir on 27/03/2019.
//

#include "uthreads.h"
#include "thread.h"

int Thread::num_of_threads = 0;

Thread::Thread(void (*f)(void) = nullptr)
{
	num_of_threads++;

	sp = (address_t) stack + STACK_SIZE - sizeof(address_t);
	pc = (address_t) f;
	sigsetjmp(env[0], 1);
	(env[0]->__jmpbuf)[JB_SP] = translate_address(sp);
	(env[0]->__jmpbuf)[JB_PC] = translate_address(pc);
	sigemptyset(&env[0]->__saved_mask);
}

/* A translation is required when using an address of a variable.
   Use this as a black box in your code. */
address_t thread::translate_address(address_t addr)
{
	address_t ret;
	asm volatile("xor    %%fs:0x30,%0\n"
				 "rol    $0x11,%0\n"
	: "=g" (ret)
	: "0" (addr));
	return ret;
}