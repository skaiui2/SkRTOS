
#include "event.h"
#include "heapmem.h"
#include "port.h"
#include "task.h"
#include "atomic.h"


event *event_creat()
{
    event *creat = heap_malloc(sizeof (event));
    *creat = (event) {
            .type = 0
    };
    listInit(&(creat->WaitList));

    return creat;
}



void event_delete(event *Event)
{
    heap_free(Event);
}


extern volatile uint8_t PendSVFlag;
extern Clocks AbsoluteTime;
extern TCB_struct *CurrentTCB;



static void up_op(event *Event)
{
    TCB_struct *ReceiveTask;
    //Wake up the highest priority task in the receiving list
    ReceiveTask =  container_of(Event->WaitList.tail, TCB_struct, IPC_node);
    DelaylistRemove(ReceiveTask);//!!!timer
    Remove_IPC(ReceiveTask);
    ReadyListAdd(ReceiveTask);

}

uint32_t event_send( event *Event, uint32_t set)
{
    uint32_t xre = xEnterCritical();
    uint32_t happened_bits = 0;
    uint32_t event_bits = Event->WaitList.tail->value & another_happen_flag;
    Event->type |= set;

    while(Event->WaitList.count != 0){
        if( (Event->WaitList.tail->value & high_four_flag) != wait_all_flag ){// only one
            if( (event_bits & Event->type) != 0 ) {
                //!!! remove  event node,set state to ready
                up_op(Event);
                //!!! if clear_flag is true,clear  event bits
                if ((Event->WaitList.tail->value & sub_four_flag) == clear_exit_flag) {
                    happened_bits |= set;
                }

            }
        } else{//need all
            if( (event_bits & Event->type) == event_bits ){
                //!!! remove  event node,set state to ready
                up_op(Event);
                //!!! if clear_flag is true,clear  event bits
                if ((Event->WaitList.tail->value & sub_four_flag) == clear_exit_flag) {
                    happened_bits |= set;
                }
            }
        }
    }
    Event->type &= ~happened_bits;

    xExitCritical(xre);
    return Event->type;
}




uint8_t event_happened(uint32_t wait_event,uint8_t clear_flag,uint8_t wait_flag)
{
    uint32_t event_value = CurrentTCB->tasknode.value;
    if( wait_flag == false ) { //only one
        if ((event_value & wait_event) != 0) {
            if (clear_flag == true) {
                CurrentTCB->tasknode.value &= ~wait_event;
            }
            return true;
        }
    }
    else { //must the all
        if ((event_value & wait_event) == wait_event) {
            if (clear_flag == true) {
                CurrentTCB->tasknode.value &= ~wait_event;
            }
            return true;
        }
    }

    return false;
}



uint8_t event_receive(event *Event,uint32_t wait_event,uint8_t clear_flag,uint8_t wait_flag,uint32_t Ticks)
{

    uint8_t RecordFlag;
    Clocks RecordClock = {0,0};
    uint32_t xre = xEnterCritical();

    uint8_t event_happen = event_happened(wait_event,clear_flag,wait_flag);
    if(event_happen == true){
        xExitCritical(xre);
        return true;
    }

    if(Ticks == 0){
        xExitCritical(xre);
        return false;
    }

    // record the longest wait time
    RecordClock.one_levelTime = AbsoluteTime.one_levelTime + Ticks;
    if (RecordClock.one_levelTime < AbsoluteTime.one_levelTime) {
        atomic_inc(&(RecordClock.two_levelTime));
    }

    if(clear_flag == true ){
        CurrentTCB->tasknode.value |= clear_exit_flag;
    }
    if(wait_flag == true ){
        CurrentTCB->tasknode.value |= wait_all_flag;
    }

    Insert_IPC(CurrentTCB,&(Event->WaitList));
    TaskDelay(Ticks);

    RecordFlag = PendSVFlag;
    xExitCritical(xre);

    //exit Critical,the PendSV will execute.
    //!!!Maybe I need to write a clock list that sorted by high priority and periodically checks to see if all tasks are obsolete.
    while (RecordFlag == PendSVFlag) {
        //wait the PendSV
    }

    uint32_t xReturn = xEnterCritical();
    event_happen = event_happened(wait_event,clear_flag,wait_flag);
    if(event_happen == true){
        xExitCritical(xReturn);
        return true;
    }

    return false;
}











