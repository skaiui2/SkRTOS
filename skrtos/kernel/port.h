#ifndef PORT_H
#define PORT_H
#include "kernel.h"



Class(Stack_register)
{
    //automatic stacking
    uint32_t xPSR;
    uint32_t PC;
    uint32_t LR;
    uint32_t r12;
    uint32_t r3;
    uint32_t r2;
    uint32_t r1;
    uint32_t r0;

    //manual stacking
    uint32_t r11;
    uint32_t r10;
    uint32_t r9;
    uint32_t r8;
    uint32_t r7;
    uint32_t r6;
    uint32_t r5;
    uint32_t r4;
};






#endif