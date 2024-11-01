#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"
#include "signal.h"


Class(queue_struct)
{
    uint8_t *startPoint;
    uint8_t *endPoint;
    uint8_t *readPoint;
    uint8_t *writePoint;
    uint8_t MessageNumber;
    thelist SendList;
    thelist ReceiveList;
    uint32_t NodeSize;
    uint32_t NodeNumber;

    signal_struct   signal_que;
    signal_impl     *interface;
};

queue_struct* xQueue_creat(uint32_t queue_length,uint32_t queue_size);
void queue_delete( queue_struct *queue );
uint8_t queue_send(queue_struct *queue, uint32_t *buf, uint32_t Ticks);
uint8_t queue_receive( queue_struct *queue, uint32_t *buf, uint32_t Ticks );


#define queue_creat( queue_length, queue_size) \
SignalCreat( (queue_length), (queue_size),(queue_type))




#endif 




