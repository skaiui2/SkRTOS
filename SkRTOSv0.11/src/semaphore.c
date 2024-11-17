#include "semaphore.h"
#include<stdint.h>
#include "heapmem.h"
#include "port.h"
#include "atomic.h"
#include "task.h"



Semaphore_struct *semaphore_creat( uint32_t SemaNumber,uint8_t type)
{
    Semaphore_struct *xSemaphore = heap_malloc(sizeof (Semaphore_struct) );
    *xSemaphore = (Semaphore_struct){
            .type   = type,
            .self   = NULL,
            .SemaNumber = SemaNumber,
            .AvailableNumber    = SemaNumber
    };
    listInit(&(xSemaphore->ReceiveList));
    return xSemaphore;
}


void semaphore_delete(Semaphore_struct *semaphore)
{
    heap_free(semaphore);
}





extern volatile uint8_t PendSVFlag;
extern Clocks AbsoluteTime;
extern TCB_struct *CurrentTCB;
static void down_op(Semaphore_struct *semaphore)
{
#if SemOwner
    semaphore->self =CurrentTCB;
#endif

    atomic_dec((uint32_t *)(&(semaphore->AvailableNumber)));
    contextSwitchTrigger();


}


//the P operation
uint8_t semaphore_down(Semaphore_struct *semaphore,uint32_t Ticks)  //get semaphore
{
    uint8_t RecordFlag;
    Clocks RecordClock = {0,0};
    uint32_t xre = xEnterCritical();
    if( semaphore->AvailableNumber > 0){
        down_op(semaphore);
        xExitCritical(xre);
        return true;
    }else if (semaphore->AvailableNumber == 0)
    {
        if (Ticks == 0)
        {
            xExitCritical(xre);
            return false;
        } else if (Ticks > 0) {
            // record the longest wait time
            RecordClock.one_levelTime = AbsoluteTime.one_levelTime + Ticks;
            if (RecordClock.one_levelTime < AbsoluteTime.one_levelTime) {
                atomic_inc(&(RecordClock.two_levelTime));
            }
        }
    }

    Insert_IPC(CurrentTCB,&(semaphore->ReceiveList));
    TaskDelay(Ticks);

    RecordFlag = PendSVFlag;
    xExitCritical(xre);
    //exit Critical,the PendSV will execute.
    //!!!Maybe I need to write a clock list that sorted by high priority and periodically checks to see if all tasks are obsolete.
    while (RecordFlag == PendSVFlag) {
        //wait the PendSV
    }

    //Block until wake up, check the time to make sure this is after wake up.
    uint32_t xReturn  = xEnterCritical();
    //check for timeout
    if ((AbsoluteTime.two_levelTime == RecordClock.two_levelTime) && (AbsoluteTime.one_levelTime <= RecordClock.one_levelTime))
    {
        down_op(semaphore);
        xExitCritical(xReturn);
        return true;
    }
    return false;

}




static void up_op(Semaphore_struct *semaphore)
{
    TCB_struct *ReceiveTask;
    //Wake up the highest priority task in the receiving list
    ReceiveTask =  container_of(semaphore->ReceiveList.tail, TCB_struct, IPC_node);
    DelaylistRemove(ReceiveTask);//!!!timer
    Remove_IPC(ReceiveTask);
    ReadyListAdd(ReceiveTask);

}


// the V operation
uint8_t semaphore_up( Semaphore_struct *semaphore)
{
    uint32_t xre = xEnterCritical();
    if (semaphore->ReceiveList.count != 0){
        up_op(semaphore);
    }
#if SemOwner
    semaphore->self = NULL;
#endif
    atomic_inc((uint32_t *)(&(semaphore->AvailableNumber)));
    //
    contextSwitchTrigger();
    xExitCritical(xre);

    return true;
}



