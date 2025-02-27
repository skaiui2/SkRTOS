
#include "mutex.h"
#include "heapmem.h"
#include "atomic.h"


xMutex *mutex_creat()
{
    xMutex *mutex = heap_malloc(sizeof (xMutex) );
    *mutex = (xMutex){
            .value = 1,
            .count = 0,
            .original_priority = 0,
            .self = NULL
    };
    listInit(&(mutex->WaitList));
    return mutex;
}



void mutex_delete(xMutex *mutex)
{
    heap_free(mutex);
}


extern volatile uint8_t PendSVFlag;
extern Clocks AbsoluteTime;
extern TCB_struct *CurrentTCB;
static void down_op(xMutex *mutex)
{
    mutex->original_priority = CurrentTCB->priority;
    mutex->self =CurrentTCB;
    atomic_dec((uint32_t *)(&(mutex->value)));
    contextSwitchTrigger();

}

// P operation
uint8_t mutex_lock(xMutex *mutex, uint32_t Ticks)
{
    TCB_struct *TopTask;
    uint8_t RecordFlag;
    Clocks RecordClock = {0,0};
    uint32_t xre = xEnterCritical();
    if( mutex->value > 0){

        down_op(mutex);
        xExitCritical(xre);
        return true;
    }else if (mutex->value == 0)
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
    //Priority inversion!
    Insert_IPC(CurrentTCB,&(mutex->WaitList));
    TopTask=container_of(mutex->WaitList.tail, TCB_struct, IPC_node);
    if( mutex->self->priority < TopTask->priority){
        mutex->self->priority = TopTask->priority;
    }

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
        down_op(mutex);
        xExitCritical(xReturn);
        return true;
    }
    return false;
}



static void up_op(xMutex *mutex)
{
    TCB_struct *ReceiveTask;
    //Wake up the highest priority task in the receiving list
    ReceiveTask =  container_of(mutex->WaitList.tail, TCB_struct, IPC_node);
    ReceiveTask->priority = mutex->original_priority;
    DelaylistRemove(ReceiveTask);//!!!timer
    Remove_IPC(ReceiveTask);
    ReadyListAdd(ReceiveTask);
}

// V operation
uint8_t mutex_unlock(xMutex *mutex)
{
    uint32_t xre = xEnterCritical();
    if (mutex->WaitList.count != 0){
        up_op(mutex);
    }
    mutex->self = NULL;
    atomic_inc((uint32_t *)(&(mutex->value)));

    contextSwitchTrigger();
    xExitCritical(xre);

    return true;
}

