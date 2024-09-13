#ifndef TASK_H
#define TASK_H
#include "kernel.h"
#include "signal.h"
#include "port.h"


typedef void (*Taskfunction)(void **self);


Class(TCB_struct)
{
    uint32_t        *topStack;
    list_node       tasknode;
    uint32_t        *startStack;
    Taskfunction    function_t;

    void (*getself_TCB)(TCB_struct* self);
    void (*getself_Task)(TCB_struct* self, Taskfunction function_t);

};


Class(thread_info)
{
    uint8_t state;
};




Class(stack_struct)
{
    Stack_register Stack_register; 
    
    void (*StackInit)( uint32_t *topStack, 
                        TCB_struct *self,    //Maybe I can define two level pointer . I can change the *pointer 
                                        //point another TCB,then I will reduce the use of memory. 
                        Taskfunction function_t );

};




Class(task_struct)
{
    TCB_struct  TCB;
    thread_info thread_info;
    stack_struct *stack_struct;
    signal_struct *signal_struct;



    void (*TaskCreat)(  Taskfunction function_t,
                        const char *const task_name,
                        uint32_t *const Startstack,
                        const   uint32_t Stack_size,
                        TCB_struct *self);
    
    void (*TaskInit)(   );
    void (*TaskReady)( );
    
    
    

};



#endif 




