//
// Created by el on 2024/11/6.
//

#include "completion.h"
#include <stdint.h>
#include "heapmem.h"
#include "port.h"
#include "atomic.h"
#include "task.h"


completion *completion_creat()
{
    completion *init_completion = heap_malloc(sizeof (completion) );
    *init_completion = (completion){
        .done = 0,
    };
    listInit(&( init_completion->WaitList) );
    return init_completion;
}



void completion_delete(completion *completion)
{
    heap_free(completion);
}



extern volatile uint8_t PendSVFlag;
extern Clocks AbsoluteTime;
extern TCB_struct *CurrentTCB;
uint8_t wait_for_completion(completion *completion, uint32_t Ticks)
{
    uint8_t RecordFlag;
    Clocks RecordClock = {0,0};
    uint32_t xre = xEnterCritical();
    if( completion->done > 0){
        atomic_dec((uint32_t*)&(completion->done));
        xExitCritical(xre);
        return true;
    }else if (completion->done == 0)
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

    Insert_IPC(CurrentTCB,&(completion->WaitList));
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
        atomic_dec((uint32_t*)&(completion->done));
        xExitCritical(xReturn);
        return true;
    }
    return false;

}

static void up_op(completion *completion)
{
    TCB_struct *WaitTask;
    //Wake up the highest priority task in the receiving list
    WaitTask =  container_of(completion->WaitList.tail, TCB_struct, IPC_node);
    DelaylistRemove(WaitTask);//!!!timer
    Remove_IPC(WaitTask);
    ReadyListAdd(WaitTask);
}

uint8_t complete(struct completion *completion)
{
    uint32_t xre = xEnterCritical();
    if (completion->WaitList.count != 0){
        up_op(completion);
    }

    atomic_inc((uint32_t *)(&(completion->done)));
    
    contextSwitchTrigger();
    xExitCritical(xre);

    return true;
}

uint8_t complete_all(struct completion *completion)
{
    uint32_t xre = xEnterCritical();
    if (completion->WaitList.count != 0){
        for(uint8_t i = 0;i < completion->WaitList.count; i++){
            up_op(completion);
        }
    }

    atomic_inc((uint32_t *)(&(completion->done)));

    contextSwitchTrigger();
    xExitCritical(xre);

    return true;
}

