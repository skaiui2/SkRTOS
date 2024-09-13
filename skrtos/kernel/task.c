
#include "task.h"
#include "list.h"






uint32_t *TaskInit(


                    )
{
    TCB_Init();
    StackInit();
    ReadylistInit();


}




uint32_t *TaskCreat(  Taskfunction function_t,
                        const char *const task_name,
                        uint32_t *const Startstack,
                        const   uint32_t Stack_size,
                        TCB_struct *self
                        )
{


}



