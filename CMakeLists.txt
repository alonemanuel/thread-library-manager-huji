cmake_minimum_required(VERSION 3.12)
project(ex2)

set(CMAKE_CXX_STANDARD 14)

add_executable(main.cpp uthreads.cpp uthreads.h thread.h thread.cpp handler.cpp
        sleeping_threads_list.cpp sleeping_threads_list.h)