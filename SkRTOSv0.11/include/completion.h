//
// Created by el on 2024/11/6.
//

#ifndef XSKRTOS_COMPLETION_H
#define XSKRTOS_COMPLETION_H


#include "signal.h"

Class(completion_struct)
{
    signal_struct   completion;
    signal_impl     *interface;
};

Class(completion)
{
    uint32_t done;
    thelist WaitList;
};


#endif //XSKRTOS_COMPLETION_H
