#ifndef CLASS_H
#define CLASS_H
#include <stddef.h>  // for offsetof

#define Class(class)    \
typedef struct  class  class;\
struct class

typedef void (* Taskfunction)(void * self);

//get father struct address
//how to use it:struct parent *parent_ptr = container_of(child_ptr, struct parent, child)
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))



#define implement(fun)  typedef void (*fun)(void *parameter)
#define false   0
#define true    1


#endif