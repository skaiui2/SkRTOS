#ifndef LIST_H
#define LIST_H

#include<stddef.h>
#include "class.h"
#include "kernel.h"


implement(Task);


Class(list_operation)
{
    void (*Init)(   thelist *xlist );
    void (*add)(    thelist *xlist, list_node *newnode );
    void (*remove)( thelist *xlist, list_node *rmnode );

};


Class(Readylist)
{
    thelist Readylists[ config_max_priori ];
    operation *ope;

};


Class(Suspendlist)
{
    thelist xlist;
    operation *ope;

};

Class(Blocklist)
{
    thelist xlist;
    operation *ope;

};



/*this is public*/
Class(list_node)
{
    Task   task;
    int value;
    list_node* prev;

    list_node* next;
    
};

Class(thelist)
{
    int count;

    list_node *head;
    list_node *tail;
};




#endif