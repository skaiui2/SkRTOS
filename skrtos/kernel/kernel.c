#include "kernel.h"
#include "port.h"



void Scheduler_Init( void )
{
    ALLlistInit();
    lei_taskInit();

}

void Scheduler_start( void)
{

    vPortRaiseBASEPRI();
    xStartScheduler();
}




void *Sch_suspendTask (TCB_struct *TCB )
{


}



void *Sch_suspendTaskALL(TCB_struct *TCB)
{


}
















