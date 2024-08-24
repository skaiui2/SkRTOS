#include "list.h"


void qiucksort(list_node* firstnode)
{

}


void listInit(thelist *xlist)
{
    xlist->count = 0;
    xlist->head = NULL;
    xlist->tail = NULL;
}


void list_insertfirst(thelist *xlist, list_node *newnode)
{
    xlist->count += 1;
    xlist->head = newnode;
    xlist->tail = newnode;
    newnode->prev = xlist;
    newnode->next = xlist;



}


void list_add(thelist *xlist, list_node *newnode)
{
    if(xlist ->count== 0)
    {
        list_insertfirst(xlist,newnode);
    }
    xlist->count +=1;

    newnode->prev = xlist->tail;
    newnode->next = xlist;
    qiucksort(xlist->head);
    



}


