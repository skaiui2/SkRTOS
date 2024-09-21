#ifndef HEAPMEM_H
#define HEAPMEM_H

#include <stddef.h>
#include "error.h"



#if config_error_func
Class(mem_error)
{
    error *base;
};


error *creat();



#else



#endif


void *heap_malloc(size_t wantsize);
void heap_free(void *xret);
void printallsize();







#endif