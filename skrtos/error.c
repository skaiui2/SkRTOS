#include "error.h"




#if config_error_func
static void error_config_get(error *getimplement)
{
    getimplement->virtable->error_config();
}


#else



#endif







