#include "mempool.h"
#include "kernel.h"



Class(poolnode)
{
    uint8_t used;
    poolnode *next;
};

Class(poolhead)
{
    poolnode head;
    poolnode *tail;
    uint8_t remainode;
    size_t blocksize;
};


static const size_t heapstructSize = (sizeof(poolnode) + (size_t)(aligment_byte)) &~(aligment_byte);

void mempool_init(poolhead *thepool,void* arraystart,uint32_t allsize)
{

    poolnode *firstnode;
    uint32_t start_heap ,end_heap;
    //get start address
    start_heap = (uint32_t)arraystart;
    /*byte aligment*/
    if( (start_heap & aligment_byte) != 0){
        start_heap += aligment_byte ;
        start_heap &= ~aligment_byte;
    }

    thepool->head.next = (poolnode *)start_heap;
    thepool->head.next->used = 0;

    end_heap  = start_heap + (uint32_t)allsize ;

    
    if( (end_heap & aligment_byte) != 0){
        end_heap += aligment_byte ;
        end_heap &= ~aligment_byte;
    }
        

    thepool->tail = (poolnode*)end_heap;

    thepool->tail->used = 0;
    thepool->tail->next =NULL;

    firstnode = (poolnode*)start_heap;
    firstnode->next = thepool->tail;
    firstnode->used =0;
    
}


   
void pool_apart(poolhead *thepool, uint8_t amount,size_t apartsize)
{
    poolnode *apartnode;
    poolnode *prevnode;

    poolnode *newnode;
    size_t aligmentrequisize;

    apartsize += heapstructSize;

    if((apartsize & aligment_byte) != 0x00)
    {
        aligmentrequisize = aligment_byte + 1 - (apartsize & aligment_byte);
        apartsize += aligmentrequisize;
    }
    //taskSUS

    prevnode = thepool->head.next;
   
    
    
    while(amount != 0)
    {
        newnode = (poolnode*)(((size_t)prevnode) + apartsize);
       
        newnode->used = 0;
        prevnode->next = newnode;
    
        prevnode = newnode;

        amount--;
    }

    thepool->remainode = amount;
    

    //I will write the taskresume
    
}


void mempool_creat(poolhead *thepool,void* arraystart,uint16_t size,uint8_t amount)
{
    uint32_t allsize = (uint32_t)amount *  ( (uint32_t)size + (uint32_t)heapstructSize);
    mempool_init(thepool,arraystart,allsize);

    pool_apart(thepool,amount,size);
   

}


void *mempool_apl(poolhead *thepool)
{
    poolnode *usenode;

    void *xReturn = NULL;

    usenode = thepool->head.next;
    
    //the next is valid ?
    while(((usenode->used) != 0 ) && (usenode->next->next != NULL))
    {
        usenode = usenode ->next;
    }
    thepool->remainode -= 1;

    xReturn = (void*)(((uint8_t*)usenode) + heapstructSize);
    return xReturn;



}



void mempool_free(void *xret)
{
    poolnode *FreeBlock;
    void *xFree = (void*)xret - (void*)heapstructSize;
    
    FreeBlock = (void*)xFree;

    FreeBlock->used = 0;

}

/*every poolnode plus heapstruct(24) eqaul  pool1 array size*/

/*

uint8_t pool1[900];
poolhead onepool;


int main()
{
    mempool_creat( &onepool,pool1, 79,10);
    void *a = mempool_apl(&onepool);
    mempool_free(a);
   return 0;
}
*/


