#include "malloc.h"
#ifndef CLASS_H
#define CLASS_H

#define Class(class)    \
typedef struct  class  class;\
struct class 


#define implement(task)  typedef int (*task)(int)


#endif