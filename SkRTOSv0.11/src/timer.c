//
// Created by el on 2024/11/15.
//
#include "timer.h"
#include "heapmem.h"
#include "task.h"
#include "atomic.h"


thelist OneClockList;
thelist TwoClockList;

thelist *ClockTicksTable;
thelist *OverClockTicksTable;

extern Clocks AbsoluteTime;
extern uint32_t NowTickCount;


void ClockListAdd( Timer_struct *timer)
{
    const uint32_t constTicks = NowTickCount;
    uint32_t wakeTime = constTicks + timer->Timer_node.value;
    timer->Timer_node.value = wakeTime;

    if( wakeTime < constTicks)
    {
        list_add( OverClockTicksTable , &(timer->Timer_node) );
    }
    else{
        list_add( ClockTicksTable , &(timer->Timer_node) );
    }
}




Timer_struct *timer_creat(const char *name, uint32_t period, timerfun CallBackFun, uint8_t timer_flag)
{
    Timer_struct *timer = heap_malloc(sizeof(Timer_struct));
    *timer = (Timer_struct){
        .Name = name,
        .TimerPeriod = period,
        .CallBackFun = CallBackFun,
        .timer_stop_flag = timer_flag
    };
    timer->Timer_node.value = period;

    ClockListAdd(timer);
    return timer;
}


void SwitchClock()
{
    thelist *temp;
    temp = ClockTicksTable;
    ClockTicksTable = OverClockTicksTable;
    OverClockTicksTable = temp;
}


uint8_t timer_check()
{
    while (1) {
        list_node *node = ClockTicksTable->head;
        while( node->value <= NowTickCount ) {
            Timer_struct *timer = container_of( node,Timer_struct,Timer_node);
            timer->CallBackFun(timer);
            list_remove(ClockTicksTable,&timer->Timer_node);
            ClockListAdd(timer);
            if( node != ClockTicksTable->tail ) {
                node = node->next;
            }
        }
    }
}

void ClockList_init()
{
    listInit(OverClockTicksTable);
    listInit(ClockTicksTable);
    ClockTicksTable = &( OneClockList);
    OverClockTicksTable = &( TwoClockList);
}

TCB_struct *timer_init(uint8_t timer_priority)
{
    TCB_struct *timer_tcb = NULL;
    ClockList_init();
    TaskCreat((Taskfunction)timer_check,
              128,
              timer_priority,
              0,
              timer_tcb,
              &timer_tcb);
    return timer_tcb;
}

uint8_t timer_pri_set(TCB_struct *timer, uint8_t timer_priority)
{
    return timer->priority = timer_priority;
}

uint8_t timer_re_run(Timer_struct *timer)
{
    atomic_set(run, (uint32_t *)&(timer->timer_stop_flag));
    ClockListAdd(timer);
    return timer->timer_stop_flag;
}



uint8_t timer_stop(Timer_struct *timer)
{
    atomic_set(stop, (uint32_t *)&(timer->timer_stop_flag));
    list_remove(ClockTicksTable,&timer->Timer_node);
    return timer->timer_stop_flag;
}






