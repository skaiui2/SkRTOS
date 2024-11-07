
#include "queue.h"
#include<stdint.h>
#include <string.h>
#include "heapmem.h"
#include "port.h"
#include "atomic.h"
#include "task.h"


queue_struct* xQueue_creat(uint32_t queue_length,uint32_t queue_size)
{
    size_t  Qsize = (size_t)( queue_length * queue_size);
    queue_struct *xQueue = heap_malloc(sizeof (queue_struct) + Qsize);
    uint8_t *message_start = (uint8_t *)xQueue + sizeof(queue_struct);

    *xQueue = (queue_struct){
            .startPoint = message_start,
            .endPoint   = (uint8_t *)(message_start + Qsize),
            .readPoint  = (uint8_t *)( message_start + ( queue_length - 1) * queue_size ),
            .writePoint = message_start,
            .MessageNumber = 0UL,
            .NodeNumber  = queue_length,
            .NodeSize   = queue_size,
    };

    listInit(&( xQueue->SendList));
    listInit(&(xQueue->ReceiveList));
    return xQueue;
}



void queue_delete( queue_struct *queue )
{
    heap_free(queue);
}



extern Clocks AbsoluteTime;
extern TCB_struct *CurrentTCB;
extern thelist *WakeTicksTable;
extern thelist *OverWakeTicksTable;

void WriteToQueue( queue_struct *queue , uint32_t *buf)
{
    TCB_struct *ReceiveTask;
    memcpy((void *) queue->writePoint, buf, (size_t) queue->NodeSize);
    queue->writePoint += queue->NodeSize;

    if (queue->writePoint >= queue->endPoint)
    {
        queue->writePoint = queue->startPoint;
    }

    if (queue->ReceiveList.count != 0)
    {
        //Wake up the highest priority task in the receiving list
        ReceiveTask =  container_of(queue->ReceiveList.tail, TCB_struct, tasknode);
        DelaylistRemove(ReceiveTask);//!!!timer
        Remove_IPC(ReceiveTask);
        ReadyListAdd(ReceiveTask);
        //
        contextSwitchTrigger();
    }
    atomic_inc( (uint32_t *)(&(queue->MessageNumber)));
}

void ExtractFromQueue( queue_struct *queue , uint32_t *buf)
{
    TCB_struct *SendTask;
    queue->readPoint += queue->NodeSize;

    if( queue->readPoint >= queue->endPoint ){
            queue->readPoint = queue->startPoint;
    }
    memcpy( ( void * ) buf, ( void * ) queue->readPoint, ( size_t ) queue->NodeSize );

    if (queue->SendList.count != 0)
    {
        //Wake up the highest priority task in the sending list
        SendTask =  container_of(queue->SendList.tail, TCB_struct, IPC_node);
        DelaylistRemove(SendTask);//!!!timer
        Remove_IPC(SendTask);
        ReadyListAdd(SendTask);
        //
        contextSwitchTrigger();
    }
    atomic_dec((uint32_t *)(&(queue->MessageNumber)));
}


extern volatile uint8_t PendSVFlag;
uint8_t queue_send(queue_struct *queue, uint32_t *buf, uint32_t Ticks)
{
    uint8_t RecordFlag;
    Clocks RecordClock = {0,0};

    uint32_t xre = xEnterCritical();

    if (queue->MessageNumber < queue->NodeNumber)
    {
        //normally write
        WriteToQueue(queue, buf);

        xExitCritical(xre);
        return true;
    } //Block!
    else if (queue->MessageNumber == queue->NodeNumber)
    {
        if (Ticks == 0)
        {
            xExitCritical(xre);
            return false;
        } else if (Ticks > 0) {
            //record the longest wait time
            RecordClock.one_levelTime = AbsoluteTime.one_levelTime + Ticks;
            if (RecordClock.one_levelTime < AbsoluteTime.one_levelTime) {
                atomic_inc(&(RecordClock.two_levelTime));
            }

        }
    }
    //maybe need to lock the queue

    // add to send list
    Insert_IPC(CurrentTCB,&(queue->SendList));
    TaskDelay(Ticks);

    RecordFlag = PendSVFlag;
    xExitCritical(xre);
    //exit Critical,the PendSV will execute.

    while (RecordFlag == PendSVFlag) {
            //wait the PendSV
    }

    //Block until wake up, check the time to make sure this is after wake up.
    uint32_t xReturn  = xEnterCritical();
    //check for timeout
    if ((AbsoluteTime.two_levelTime == RecordClock.two_levelTime) && (AbsoluteTime.one_levelTime < RecordClock.one_levelTime))
    {
        WriteToQueue(queue, buf);
        xExitCritical(xReturn);
        return true;
    }

    return false;
}



uint8_t queue_receive( queue_struct *queue, uint32_t *buf, uint32_t Ticks )
{
    uint8_t RecordFlag;
    Clocks RecordClock = {0,0};
    uint32_t xre = xEnterCritical();
    if( queue->MessageNumber > 0){
        ExtractFromQueue(queue,buf);
        xExitCritical(xre);
        return true;
    }else if (queue->MessageNumber == 0)
    {
        if (Ticks == 0)
        {
            xExitCritical(xre);
            return false;
        } else if (Ticks > 0) {
            // record the longest wait time
            RecordClock.one_levelTime = AbsoluteTime.one_levelTime + Ticks;
            if (RecordClock.one_levelTime < AbsoluteTime.one_levelTime) {
                atomic_dec(&(RecordClock.two_levelTime));
            }
        }
    }

    Insert_IPC(CurrentTCB,&(queue->ReceiveList));
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
    if ((AbsoluteTime.two_levelTime == RecordClock.two_levelTime) && (AbsoluteTime.one_levelTime < RecordClock.one_levelTime))
    {
        ExtractFromQueue(queue, buf);
        xExitCritical(xReturn);
        return true;
    }
    return false;
}


