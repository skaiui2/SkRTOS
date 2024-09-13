#ifndef QUEUE_H
#define QUEUE_H

#include "kernel.h"
#include "list.h"
#include "signal.h"


Class(queue_struct)
{
    signal_struct   signal_que;
    signal_impl     *interface;
};





#endif 




