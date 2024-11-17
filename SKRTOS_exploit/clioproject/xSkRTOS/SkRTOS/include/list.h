#ifndef LIST_H
#define LIST_H

#include "class.h"
#include "config.h"



/*this is public*/
Class(list_node)
{
    uint32_t value;
    list_node* prev;
    list_node* next;
    void    *thelist;
};

Class(thelist)
{
    uint8_t count;
    uint8_t SwitchFlag;
    list_node *head;
    list_node *tail;
    list_node *SaveNode;
};



Class(ADT_operation)
{
    void (*Init)(   thelist *xlist );
    void (*add)(    thelist *xlist, list_node *newnode );
    void (*remove)( thelist *xlist, list_node *rmnode );

};


Class(ReadyADT)
{
    thelist Readylists[ config_max_priori ];
    ADT_operation *ope;

};


Class(DelayADT)
{
    thelist xlist;
    ADT_operation *ope;

};


Class(SuspendADT)
{
    thelist xlist;
    ADT_operation *ope;

};

Class(BlockADT)
{
    thelist xlist;
    ADT_operation *ope;

};

#define ListNodeNumber( list_struct ) \
            (uint32_t)list_struct->count

void oper_Creat(ADT_operation *ope);

void listInit(thelist *xlist);
void list_add(thelist *xlist, list_node *newnode);
void list_remove(thelist *xlist, list_node *rmnode);

void Insertfirst(thelist *xlist, list_node *newnode);
void Inserthead(thelist *xlist, list_node *newnode);
void Inserttail(thelist *xlist, list_node *newnode);
void Insertmiddle(thelist *xlist, list_node *newnode);

void remove_last( thelist *xlist, list_node *rmnode );
void remove_head( thelist *xlist, list_node *rmnode );
void remove_middle( thelist *xlist, list_node *rmnode );
void remove_tail( thelist *xlist, list_node *rmnode );





#endif