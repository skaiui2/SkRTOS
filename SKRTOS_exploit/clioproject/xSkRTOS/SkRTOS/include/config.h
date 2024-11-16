#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>
/*
 * You need some konwledge to config the size of heap:
Flash section: Mainly contains code segment (.text), initialization data segment (.data),
 this part of the data is loaded from Flash to RAM when the program is running.

RAM section: Mainly contains data segments (.data) and BSS segments (.BSS),
where bss segments are uninitialized global and static variables that are allocated space in RAM when the program is run.
We define the heap in the RAM.
*/


//the date is  always used
#define aligment_byte               0x07

#define helpValue_MAX              ( ~( ( size_t ) 0 ) )

//


#define configSysTickCLOCK_HZ			( ( unsigned long ) 72000000 )
#define configTICK_RATE_HZ			( ( uint32_t ) 1000 )

#define configShieldInterPriority 	191

#define config_heap   10*1024

#define config_error_func  0

#define config_error_func_more     0


#define config_max_priori  32


#define SemOwner 1
#define configNotifyArray   1




#endif