
#include "heapmem.h"
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include "config.h"




#define arm_max_memory    ( ~(size_t) 0 )

#define MIN_size     ((size_t) (heapstructSize << 1))     


#if config_error_func

void mema_error_help()
{
    printf("Error in function: %s at line: %d\n", __func__, __LINE__); 
    exit(-1);
}



error *creat()
{
    #if config_error_func_more

    static const error_interface *vtable[] = { mema_error_help1,  mema_error_help2};
    static error base = {
        .virtable = vtable + 1
    };

    #else

    static const error_interface vtable = {
        .error_config = mema_error_help
    };

    static error base = {
        .virtable = &vtable
    };

    #endif


    if( !( mem_error *real_mema_err = malloc(sizeof(real_mema_err)) ) )
    {
        mema_error_help();
        return -1;
    };
    if( memcpy(&real_mema_err->base,&base,sizeof(base)) == NULL )
    {
        mema_error_help();
        return -1;
    };

    return (error*)(&real_mema_err);


}


#else 

Error_help(mema) 

#endif




Class(heap_node){

    heap_node *next;
    size_t blocksize;

};


Class(xheap){
    heap_node head;
    heap_node *tail;
    size_t allsize;
    uint8_t nodecount;

};

xheap theheap = {
    
    .tail = NULL,
    .allsize = config_heap,
    .nodecount = 0
};


static  uint8_t allheap[config_heap];




static const size_t heapstructSize = (sizeof(heap_node) + (size_t)(aligment_byte)) &~(aligment_byte);



void printallsize()
{
    printf("allsize is %d\n",(int)theheap.allsize);
}

void heap_init()
{

    heap_node *firstnode;
    
    uint32_t start_heap ,end_heap;
    //get start address
    start_heap =(uint32_t) allheap;

    /*byte aligment*/
    if( (start_heap & aligment_byte) != 0){
        start_heap += aligment_byte ;
        start_heap &= ~aligment_byte;
        theheap.allsize -=  (size_t)(start_heap - (uint32_t)allheap);//byte aligment means move to high address,so sub it!
    }

    theheap.head.next = (heap_node *)start_heap;
    theheap.head.blocksize = (size_t)0;

    end_heap  = ( start_heap + (uint32_t)config_heap);


    /*byte aligment*/
    if( (end_heap & aligment_byte) != 0){
        end_heap += aligment_byte ;
        end_heap &= ~aligment_byte;
        theheap.allsize =  (size_t)(end_heap - start_heap );

    }
    

    theheap.tail = (heap_node*)end_heap;
    theheap.tail->blocksize  = 0; 
    theheap.tail->next =NULL;

    firstnode = (heap_node*)start_heap;
    firstnode->next = theheap.tail;

    firstnode->blocksize = theheap.allsize;


}



void *heap_malloc(size_t wantsize)
{
    heap_node *prevnode;
    heap_node *usenode;

    heap_node *newnode;
    size_t aligmentrequisize;

    void *xReturn = NULL;

    if(wantsize <= 0 )
    {
        printf("Error in function: %s at line: %d\n", __func__, __LINE__); \
        exit(-1); 

    }

     
    wantsize += heapstructSize;
    if((wantsize & aligment_byte) != 0x00)
    {
        aligmentrequisize = aligment_byte - (wantsize & aligment_byte);
        wantsize += aligmentrequisize;
    }


    if((wantsize / arm_max_memory) != 0)
    {
        printf("Error in function: %s at line: %d\n", __func__, __LINE__); \
        exit(-1); 

    }
     
    //I will add the TaskSuspend function ,that make there be a atomic operation
    if(theheap.tail== NULL )
    {
        heap_init();
    }

    //

    prevnode = &theheap.head;
    usenode = theheap.head.next;
   

    //the next is valid ?

    while((usenode->blocksize) < wantsize )
    {
        prevnode = usenode;
        usenode = usenode ->next;
    }

    //

    if(usenode == theheap.tail)
    {
        printf("Error in function: %s at line: %d\n", __func__, __LINE__); \
        exit(-1);
    }
    
    xReturn = (void*)(((uint8_t*)usenode) + heapstructSize);

    prevnode->next = usenode->next ;
    /*apart it!*/
    if( (usenode->blocksize - wantsize) > MIN_size )
    {
        newnode = (void*)(((uint8_t*)usenode) + wantsize);

        newnode->blocksize = usenode->blocksize - wantsize;
        usenode->blocksize = wantsize;
        

        newnode->next = prevnode->next ;
        prevnode->next = newnode;
        
    }
   
    
    theheap.allsize-= usenode->blocksize;

    

    if(theheap.allsize < 0U)
    {
        printf("Error in function: %s at line: %d\n", __func__, __LINE__); \
        exit(-1);
    }
    
    usenode->next = NULL;

    // I will write the TaskResume



    

    if(usenode > theheap.tail)
    {

        printf("alheap : %d ,use: %d, tail: %d\n",allheap,usenode,theheap.tail);
        printf("fuck : %s at line: %d\n", __func__, __LINE__); 
        exit(-1);
    }

    
    return xReturn;


}

static void InsertFreeBlock(heap_node* xInsertBlock);

void heap_free(void *xret)
{
    uint8_t *xFree = (uint8_t*)xret;
    heap_node *xlink;

    if(xFree == NULL)
    {
        printf("Error in function: %s at line: %d\n", __func__, __LINE__); \
        exit(-1);
    }

    xFree -= heapstructSize;
    xlink = (void*)xFree;
    if(xlink->next != NULL)
    {
        printf("Error in function: %s at line: %d\n", __func__, __LINE__); \
        exit(-1);
    }

    // I will tasksuspend
    theheap.allsize += xlink->blocksize;
    InsertFreeBlock((heap_node*)xlink);
     
    // I will TaskResume




}

#define chose 0

static void InsertFreeBlock(heap_node* xInsertBlock)
{
    heap_node *first_fitnode;
    uint8_t* getaddr;

    for(first_fitnode = &theheap.head;first_fitnode->next < xInsertBlock;first_fitnode = first_fitnode->next)
    {
        /* this is first fit!*/
    }
    
#if chose
    getaddr = (uint8_t*)first_fitnode;
    if(getaddr + first_fitnode->blocksize == (uint8_t*) xInsertBlock)
    {
       first_fitnode->blocksize += xInsertBlock->blocksize;
       xInsertBlock = first_fitnode;
    }       
   
    getaddr = (uint8_t*)xInsertBlock;
    if((getaddr + xInsertBlock->blocksize) == (uint8_t*)(first_fitnode->next))
    {
        if(first_fitnode->next != theheap.tail )
        {
            xInsertBlock->blocksize += first_fitnode->next->blocksize;
            xInsertBlock->next = first_fitnode->next->next;
        }
        else
        {
            xInsertBlock->next = theheap.tail;
        }        
    }
    else
    {
        xInsertBlock->next = first_fitnode->next;
        
    }
    
    if(first_fitnode != xInsertBlock)
    {
        first_fitnode->next = xInsertBlock;
    }
    

#else

    xInsertBlock->next = first_fitnode->next;
    first_fitnode->next = xInsertBlock;

    getaddr = (uint8_t*)xInsertBlock;
    if((getaddr + xInsertBlock->blocksize) == (uint8_t*)(xInsertBlock->next))
    {
      
        if(xInsertBlock->next != theheap.tail )
        {
            xInsertBlock->blocksize += xInsertBlock->next->blocksize;
            xInsertBlock->next = xInsertBlock->next->next;
        }
        else
        {
            xInsertBlock->next = theheap.tail;
        }
    }
    
    
    
    getaddr = (uint8_t*)first_fitnode;

    if((getaddr + first_fitnode->blocksize) == (uint8_t*) xInsertBlock)
    {
        first_fitnode->blocksize += xInsertBlock->blocksize;
        first_fitnode->next = xInsertBlock->next;
    }

#endif
}


/*

int main()
{
    void *a =heap_malloc(sizeof(int));
}


*/
