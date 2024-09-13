#ifndef SIGNAL_H
#define SIGNAL_H
#include"kernel.h"
#include"list.h"


typedef uint8_t (*signal_function)(signal_struct **self);

Class(signal_struct)
{
    uint8_t *messageStart;
    uint8_t *messageEnd;
    list_node pendingtask;
    
};

Class(signal_impl)
{
    signal_function implementation;
};













#endif 