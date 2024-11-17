
#ifndef EVENT_H
#define EVENT_H

#include "signal.h"


#define clear_exit_flag         0x01000000UL
#define wait_all_flag           0x10000000UL


#define another_four_flag           0x0fffffffUL
#define high_four_flag              0xf0000000UL
#define subordinate_four_flag       0xf0ffffffUL
#define sub_four_flag               0x0f000000UL

#define event_happen_flag           0xff000000UL
#define another_happen_flag         0x00ffffffUL



Class(event_struct)
{
    signal_struct   event_mut;
    signal_impl     *interface;
};

Class(event)
{
    uint32_t type;
    thelist WaitList;
};


#endif 