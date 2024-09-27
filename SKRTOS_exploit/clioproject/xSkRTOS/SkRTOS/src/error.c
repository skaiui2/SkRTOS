#include "error.h"




#if config_error_func
static void error_config_get(error *getimplement)
{
    getimplement->virtable->error_config();
}

void error_panic(void)
{
    while(1){

    }
}


#else



#endif







