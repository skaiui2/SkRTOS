

#ifndef MUTEX_H
#define MUTEX_H

#include "signal.h"
#include "list.h"

Class(mutex_struct)
{
    signal_struct   signal_mut;
    signal_impl     *interface;
};


#endif 