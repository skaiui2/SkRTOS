

#ifndef MUTEX_H
#define MUTEX_H

#include "signal.h"
#include "task.h"

Class(mutex_struct)
{
    signal_struct   signal_mut;
    signal_impl     *interface;
};

Class(xMutex)
{
    uint8_t        value;
    uint32_t          original_priority;
    uint32_t          count;
    thelist          WaitList;
    TCB_struct      *self;
};



#endif 