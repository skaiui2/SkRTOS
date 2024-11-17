//
// Created by el on 2024/9/24.
//

#include "sorter.h"
#include "task.h"


Class(clock_list)
{
    list_node listNode;
    Clocks clock;
};

Class(TheClock){
    thelist xlist;
    clock_list *clock_head;
    clock_list *clock_tail;
};


uint8_t sorter_control(TheClock *xclock , clock_list *node)
{

    uint8_t rt = 0;

    if(xclock->xlist.count== 0)       return rt;
    rt += 1;

    if( node->clock.two_levelTime <= xclock->clock_head->clock.two_levelTime) {
        if(node->clock.one_levelTime <= xclock->clock_head->clock.one_levelTime){
            return rt;
        }
    }

    rt += 1;

    if( node->clock.two_levelTime >= xclock->clock_tail->clock.two_levelTime)
    {
        if(node->clock.one_levelTime >= xclock->clock_tail->clock.one_levelTime){
            return rt;
        }
    }
    rt += 1;

    if( ( ( node->clock.two_levelTime) > (xclock->clock_head->clock.two_levelTime)  )
    && (node->clock.two_levelTime < xclock->clock_tail->clock.two_levelTime) )  {
        if(( ( node->clock.one_levelTime) > (xclock->clock_head->clock.one_levelTime)  )
           && (node->clock.one_levelTime < xclock->clock_tail->clock.one_levelTime)){
            return rt;
        }
    }
}


void sorter_dataflow(uint8_t rt,thelist *xlist, list_node *newnode)
{
    void (*listInsert[])(thelist *xlist, list_node *newnode) = {
            Insertfirst,
            Inserthead,
            Inserttail,
            Insertmiddle,
    };
    listInsert[rt](xlist, newnode);

}



void sorter_add(TheClock *xclock, clock_list *node)
{
    uint8_t op = sorter_control(xclock,node);
    sorter_dataflow(op,&xclock->xlist, &node->listNode);
    xclock->xlist.count += 1;
}

