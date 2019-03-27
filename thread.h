//
// Created by kalir on 27/03/2019.
//

#ifndef EX2_THREAD_H

#include "uthreads.h"

#define EX2_THREAD_H
#define READY 0
#define BLOCKED 1
#define RUNNING 2

class Thread{

private:
    static int num_of_threads;

protected:
    int _state;
    int _stack_size;
    int _id;
    void (*func)(void);

public:
    Thread(void (*f)(void) = nullptr): _state(READY), _stack_size(STACK_SIZE), _id(num_of_threads), func(f){
        num_of_threads++;
    }

    int get_id(){return _id;};

    int get_state(){return _state;};

    void set_state(int state){_state = state;};
};

#endif //EX2_THREAD_H
