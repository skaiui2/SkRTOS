//
// Created by el on 2024/11/15.
//


#include "notify.h"
#include "heapmem.h"

extern volatile uint8_t PendSVFlag;
extern TCB_struct *CurrentTCB;
Notify_struct *notify_creat(uint32_t *value, uint8_t *type)
{
    Notify_struct *notify = heap_malloc(sizeof(Notify_struct));
    for(uint8_t i = 0 ; i < configNotifyArray ; i++)
    {
        notify->value[i] = value[i];
        notify->state[i] = type[i];
    }

    listInit(&(notify->wait_list));
    notify->self = CurrentTCB;
    return notify;
}



void notify_delete(Notify_struct *notify)
{
    heap_free(notify);
}

static inline uint32_t NoType(Notify_struct *notify, uint8_t index, uint32_t value, uint8_t state)
{

}

static inline uint32_t SetBit(Notify_struct *notify, uint8_t index, uint32_t value, uint8_t state)
{
    return notify->value[index] |= value;
}

static inline uint32_t Inc(Notify_struct *notify, uint8_t index, uint32_t value, uint8_t state)
{
    return (notify->value[index])++;
}

static inline uint32_t Write(Notify_struct *notify, uint8_t index, uint32_t value, uint8_t state)
{
    return notify->value[index] = value;
}



static inline uint32_t OverWrite(Notify_struct *notify, uint8_t index, uint32_t value, uint8_t state)
{
    if(state != OBTAINED){
        return notify->value[index] = value;
    }
    notify_Error();
}

 static inline void operation(Notify_struct *notify, uint8_t index, uint32_t value, uint8_t state,uint8_t type)
{
    uint32_t (*date_flow[])(Notify_struct *notify, uint8_t index, uint32_t value, uint8_t state) = {
        NoType,
        SetBit,
        Inc,
        Write,
        OverWrite
    };
    date_flow[type](notify, index, value, state);

}


uint8_t notify_send(Notify_struct *notify, uint8_t index, uint32_t value, eType type)
{
    uint32_t xre = xEnterCritical();
    uint8_t origin_state = notify->state[index];
    operation(notify, index, value, origin_state, type);
    notify->state[index] = OBTAINED;
    if( origin_state == WAITING){
        Remove_IPC(notify->self);
        ReadyListAdd(notify->self);
        contextSwitchTrigger();
    }

    xExitCritical(xre);
    return true;
}


uint8_t notify_wait(Notify_struct *notify, uint8_t index, uint32_t ClearEntry, uint32_t ClearExit, uint32_t Ticks)
{
    uint8_t RecordFlag;
    uint32_t xre = xEnterCritical();

    if( notify->state[index] != OBTAINED ) {
        notify->value[index] &= ~ClearEntry;
        notify->state[index] = WAITING;

        if (Ticks > (uint32_t) 0) {
            Insert_IPC(CurrentTCB, &(notify->wait_list));
            TaskDelay(Ticks);
            contextSwitchTrigger();
        }else{
            return false;
        }
    }
    RecordFlag = PendSVFlag;
    xExitCritical(xre);

    //exit Critical,the PendSV will execute.
    //!!!Maybe I need to write a clock list that sorted by high priority and periodically checks to see if all tasks are obsolete.
    while (RecordFlag == PendSVFlag) {
        //wait the PendSV
    }

    uint32_t xReturn = xEnterCritical();
    if( notify->state[index] != OBTAINED ) {
        return false;
    }
    else {
        notify->value[index] &= ~ClearExit;
    }
    notify->state[index] = LEISURE;

    xExitCritical(xReturn);
    return true;
}







