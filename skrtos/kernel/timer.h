//
// Created by el on 2024/11/15.
//

#ifndef XSKRTOS_TIMER_H
#define XSKRTOS_TIMER_H

#include "list.h"

#define run     0
#define stop    1

implement(timerfun);


Class(Timer_struct)
{
    const char      *Name;
    list_node       Timer_node;
    uint32_t        TimerPeriod;
    timerfun        CallBackFun;
    uint8_t         timer_stop_flag ;
};
void SwitchClock();







#endif //XSKRTOS_TIMER_H
