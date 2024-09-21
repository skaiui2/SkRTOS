#include "list.h"





void listInit(thelist *xlist)
{
    
    xlist->count = 0;
    xlist->head = NULL;
    xlist->tail = NULL;
    
}


void Insertfirst(thelist *xlist, list_node *newnode)
{
    xlist->count += 1;
    xlist->head = newnode;
    xlist->tail = newnode;
    newnode->prev = xlist;
    newnode->next = xlist;

}


void Inserthead(thelist *xlist, list_node *newnode)
{
    newnode->prev = (list_node*)xlist;
    newnode->next = xlist->head;

    xlist->head->prev = newnode;

    xlist->head = newnode;

    xlist->count +=1;
}

void Inserttail(thelist *xlist, list_node *newnode)
{
    newnode->prev = xlist->tail;
    newnode->next = xlist;

    xlist->tail->next = newnode;

    xlist->tail = newnode;

    xlist->count +=1;
     
 
}


void Insertmiddle(thelist *xlist, list_node *newnode)
{
    list_node *findhelp;

    for(findhelp = xlist->head;findhelp->next->value <= newnode->value; findhelp = findhelp->next)
    {
        //finding...
    }

    newnode->prev = findhelp;
    newnode->next = findhelp->next;

    findhelp->next->prev = newnode;

    findhelp->next = newnode;

    xlist->count +=1;

}

/* use the table to record all condition,separate the judgment conditions (like if else)  and excution

*/

uint8_t control(thelist *xlist , list_node *newnode)
{

    uint8_t rt = 0;
    
    if(xlist ->count== 0)       return rt;
    rt += 1;

    if( newnode->value <= xlist->head->value)   return rt;
    rt += 1;

    if( newnode->value > xlist->tail->value)    return rt;
    rt += 1;

    if(( newnode->value > xlist->head->value  ) && (newnode->value < xlist->tail->value) )  return rt;

}


void dataflow(uint8_t rt,thelist *xlist, list_node *newnode)
{
    void (*listInsert[])(thelist *xlist, list_node *newnode) = {
        Insertfirst,    
        Inserthead,    
        Insertmiddle,
        Inserttail
    };
    listInsert[rt](xlist, newnode);

}



/*for same priority task,the task that hava minster usetime ,it will be excute early.greedy algorithm!

*/
void list_add(thelist *xlist, list_node *newnode)
{
    
    uint8_t op = control(xlist,newnode);
    dataflow(op,xlist, newnode);

}
    





void list_remove(thelist *xlist, list_node *rmnode)
{
    rmnode->prev->next = rmnode->next;

    if(rmnode != xlist->tail)
    {
        rmnode->next->prev = rmnode->prev;
    }
    else
    {
        xlist->tail = rmnode->prev;
    }

    xlist->count -=1;

}

void oper_Init(list_operation *ope)
{
    ope->Init = (void *)listInit;
    ope->add  = (void *)list_add;
    ope->remove =(void *) list_remove;

}




static thelist Readytasklist[ config_max_priori ];


list_node newreadynode0;
list_node newreadynode1;
list_node newreadynode2;
list_node newreadynode3;
list_node newreadynode4;
list_node newreadynode5;




int main()
{
    list_operation ope;
    oper_Init(&ope);
    newreadynode0.value = 3;


    listInit(&Readytasklist[0]);
    ope.add(&Readytasklist[0],&newreadynode0);

    
    //printf("the:%d\n",(&Readytasklist[0])->count);

    return 0;
}









