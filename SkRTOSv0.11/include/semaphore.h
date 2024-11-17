#ifndef SEMAPHORE_H
#define SEMAPHORE_H



#include "list.h"
#include "signal.h"
#include "queue.h"
#include "task.h"

Class(sem_struct)
{
    signal_struct signal_sem;
    signal_impl *interface;

};


Class(Semaphore_struct)
{
    uint8_t type;
    TCB_struct *self;
    uint32_t SemaNumber;
    thelist ReceiveList;
    uint32_t AvailableNumber;
};


/*there are two Semaphore,one is owner Semaphore, another is none owner Semaphore.
 * The difference is whether to create a semaphore  with the init value of 0 or 1.
 * */

//Owner Semaphore,There is no sender and receiver.Must take Semaphore then release it!
#define SemBinary  0
#define SemCount    1


#define SemBinaryCreat() \
    semaphore_creat( 1, SemBinary)


#define SemCountCreat( SemaNumber) \
    semaphore_creat( SemaNumber, SemCount)

void semaphore_delete(Semaphore_struct *semaphore);
uint8_t semaphore_up( Semaphore_struct *semaphore);
uint8_t semaphore_down(Semaphore_struct *semaphore,uint32_t Ticks);



//Sender and receiver Semaphore. The semaphore has no concept of owner.


#if SemOwner
#define xSemBinaryCreat() \
    semaphore_creat( 0, SemBinary)

#define xSemCountCreat( SemaNumber) \
    semaphore_creat( SemaNumber, SemCount)

#define xSemaphore_delete() \
    semaphore_delete( semaphore )

#define xSemaphoreGive( semaphore ) \
    semaphore_up(semaphore)

#define xSemaphoreTake( semaphore ) \
    semaphore_down( semaphore, Ticks )


#endif







#endif
