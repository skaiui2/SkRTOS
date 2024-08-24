#ifndef ERROR_H
#define ERROR_H


#include "kernel.h"


#if config_error_func

Class(error)
{
    static const error_interface* const virtable;

};


Class(error_interface)
{
    void (*error_config)();
};


void error_config_get(error *getimplement);

#else 

#define Error_help(source) \
void source##_Error() \
{ \
    printf("Error in function: %s at line: %d\n", __func__, __LINE__); \
    exit(-1);  \
}


#endif






#endif