#ifndef TASK_H
#define TASK_H
#include "list.h"
#include "signal.h"
#include "port.h"


Class(TCB_struct)
{
    volatile uint32_t        *topStack;
    uint32_t        *startStack;
    list_node       tasknode;
    list_node       IPC_node;
    Taskfunction    function_t;
    uint8_t         priority;
    uint8_t         TimeSlice;

};
typedef TCB_struct* TaskHandle;


Class(stack_struct)
{
    Stack_register *Stack_register;

    void (*Stack_pre)();
    void (*Stack_next)();
};


/*
    In SkRTOS.there are six states:
    Number 0, Running state: the running task,it's priority must is highest.
    Number 1, Ready state: The task is in the ready list and runs when other tasks priority are less.
    Number 2, Access state: wait to access some hardware resources.
    Number 3, Blocked state: wait until something happen.
    Number 4, Suspend state: won't be using it for a long time.
    Number 5, Dead state: This task is no longer of any use,the os must reclaim it.
 */
Class(thread_info)
{
    uint8_t state;
    void (*print_info)( stack_struct *stack);
};





Class(task_struct)
{
    TCB_struct  *TCB;
    stack_struct *stackStruct;
    thread_info *threadInfo;
    signal_struct *signalStruct;

    void (*TaskCreat)( );
    void (*TaskStateSet)( );

};

Class(Clocks)
{
    uint32_t one_levelTime;
    uint32_t two_levelTime;
};






//function that used by other source file
void ALLlistInit();
void lei_taskInit( void );


//API
void *TaskCreat(  Taskfunction function_t,
                  const   uint16_t Stack_size,
                  uint8_t priority,
                  uint8_t TimeSlice,
                  void * const pvParameters,
                  TaskHandle * const self
);

void TaskDelay( const uint16_t TicksDelay );
void TaskDelayTimeCheck( void );
void vTaskSwitchContext( void );
void ReadyListAdd( TCB_struct *self );
void ReadyListRemove( TCB_struct *self );
void Insert_IPC(TCB_struct *self,thelist *IPC_list);
void Remove_IPC(TCB_struct *self);
void DelaylistAdd( TCB_struct *self);
void DelaylistRemove( TCB_struct *self);
uint8_t cpu_use_rate();














#endif 




