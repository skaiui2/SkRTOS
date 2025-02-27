#ifndef ERROR_H
#define ERROR_H


#include "class.h"


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
void error_panic(void);

#else 

#define Error_help(source) \
void source##_Error() \
{   \
    while(1){               \
                            \
    }                       \
}


#endif






#endif