cmake_minimum_required(VERSION 3.12)
project(ex2)

set(CMAKE_CXX_STANDARD 14)

add_executable(ex2 uthreads.cpp uthreads.h
        sleeping_threads_list.cpp sleeping_threads_list.h
        thread.h thread.cpp test3.cpp)