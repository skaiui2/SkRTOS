#ifndef LIST_H
#define LIST_H

#include<stddef.h>
#include "class.h"


implement(Task);

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
}








#endif