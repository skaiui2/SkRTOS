//
// Created by el on 2024/9/24.
//

#ifndef XSKRTOS_SORTER_H
#define XSKRTOS_SORTER_H
#include "class.h"
#include "task.h"

Class(sorter)
{


    void (*sFindHighest)( void );
    void (*sErgodicAllNode)( void );
    void (*sorterAddNode)( TCB_struct *self);
    void (*sorterDeleteNode)( TCB_struct *self);
    void (*sorterFlag)( void);
};



#endif //XSKRTOS_SORTER_H
