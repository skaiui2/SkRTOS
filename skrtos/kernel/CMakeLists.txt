cmake_minimum_required(VERSION 3.28)
project(kernel C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(kernel
        class.h
        config.h
        error.c
        error.h
        event.c
        event.h
        heapmem.c
        heapmem.h
        kernel.c
        kernel.h
        list.c
        list.h
        mempool.c
        mempool.h
        mutex.c
        mutex.h
        port.c
        port.h
        queue.c
        queue.h
        semaphore.c
        semaphore.h
        siganl.c
        signal.h
        task.c
        task.h)
