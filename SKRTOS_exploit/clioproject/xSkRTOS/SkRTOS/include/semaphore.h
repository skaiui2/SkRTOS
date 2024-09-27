#ifndef SEMAPHORE_H
#define SEMAPHORE_H



#include "list.h"
#include "signal.h"

Class(sem_struct)
{
    signal_struct signal_sem;
    signal_impl *interface;

};




#endif /
