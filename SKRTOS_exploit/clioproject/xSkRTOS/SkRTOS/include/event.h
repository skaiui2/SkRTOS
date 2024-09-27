
#ifndef EVENT_H
#define EVENT_H

#include "signal.h"
#include "list.h"

Class(event_struct)
{
    signal_struct   event_mut;
    signal_impl     *interface;
};


#endif 