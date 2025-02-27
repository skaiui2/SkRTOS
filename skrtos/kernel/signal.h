#ifndef SIGNAL_H
#define SIGNAL_H
#include"config.h"
#include"list.h"
#include <stdint.h>


#define queue_type  0
#define sem_type    1
#define mutex_type  2



Class(signal_struct)
{
    uint8_t *messageStart;
    uint8_t *messageEnd;
    list_node pendingtask;
    
};

typedef uint8_t (*signal_function)(signal_struct **self);






Class(signal_impl)
{
    signal_function implementation;
};













#endif 