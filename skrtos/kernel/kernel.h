#ifndef KERNEL_H
#define KERNEL_H
#include "class.h"
#include <stdint.h>
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
//only commom header file!
#include "task.h"

//the date is  always used  
#define aligment_byte               0x07

#define helpValue_MAX              ( ~( ( size_t ) 0 ) )



Class(scheduler)
{
    void (*Sch_Start)(void);
    void (*Sch_suspendTask)(TCB_struct *TCB );
    void (*Sch_suspendTaskALL)(TCB_struct *TCB);
    void (*Sch_BlockTask)(TCB_struct *TCB);
    


}






#endif