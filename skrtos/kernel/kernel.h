#ifndef KERNEL_H
#define KERNEL_H

#include "class.h"
#include "task.h"



Class(scheduler)
{

    void (*Sch_Start)(void);
    void (*Sch_findhighest)();
    void (*Sch_switch)();

    void (*Sch_suspendTask)( TCB_struct *TCB );
    void (*Sch_suspendTaskALL)(TCB_struct *TCB);
    void (*Sch_BlockTask)(TCB_struct *TCB);
};

void Scheduler_Init( void );
void Scheduler_start( void );




#endif