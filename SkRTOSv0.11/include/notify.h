//
// Created by el on 2024/11/15.
//

#ifndef XSKRTOS_NOTIFY_H
#define XSKRTOS_NOTIFY_H

#include "list.h"
#include "signal.h"
#include "task.h"

Class(Notify_struct)
{
    volatile uint32_t value[configNotifyArray];
    volatile uint8_t state[configNotifyArray];
    thelist wait_list;
    TCB_struct *self;
};

#define LEISURE         0
#define WAITING         1
#define OBTAINED        2

typedef enum
{
    eNoType = 0,
    eSetBit,
    eInc,
    eWrite,
    eOverWrite
} eType;

Error_help(notify)













#endif //XSKRTOS_NOTIFY_H
