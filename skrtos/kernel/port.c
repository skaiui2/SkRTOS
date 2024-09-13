#include "port.h"
#include "kernel.h"
#include "task.h"


Error_help(port)









#define Init_XPSR   0x01000000L
#define ThumbFun_adderss                ( ( uint32_t ) 0xfffffffeUL )

#define LR_error      port_Error


uint32_t * portInit_Stack(  uint32_t * topStack,
                            TCB_struct *self,
                            Taskfunction function_t )
{
    Stack_register *Stack;
    uint32_t *stackReg;
    stackReg = (uint32_t)(topStack) & (~((uint32_t)aligment_byte));
    Stack = (Stack_register *)stackReg;

    Stack->xPSR = Init_XPSR;                                   
    
    Stack->PC = ( ( uint32_t ) function_t ) & ThumbFun_adderss; 
    
    Stack->LR = ( uint32_t ) LR_error;             
                                                     
    Stack->r0 = ( uint32_t ) self;    

    topStack = (uint32_t*)(Stack->r4)--;                    
                                                       

    return topStack;
}



void xSVCHandler( void )
{
    
    __asm volatile (

        "   ldr r3,  =presentTCB      \n" /* Restore the context. */
        "   ldr r1, [r3]                    \n" /* Use pxCurrentTCBConst to get the pxCurrentTCB address. */
        "   ldr r0, [r1]                    \n" /* The first item in pxCurrentTCB is the task top of stack. */
        "   ldmia r0!, {r4-r11}             \n" /* Pop the registers that are not automatically saved on exception entry and the critical nesting count. */
        "   msr psp, r0                     \n" /* Restore the task stack pointer. */
        "   isb                             \n"
        "   mov r0, #0                      \n"
        "   msr basepri, r0                 \n"
        "   orr r14, #0xd                   \n"
        "   bx r14                          \n"
        "                                   \n"
        "   .align 4                        \n"
        "pxCurrentTCBConst2: .word pxCurrentTCB             \n"
        );
}
/*-----------------------------------------------------------*/

static void xStartFirstTask( void )
{
    __asm volatile (
        " ldr r0, =0xE000ED08   \n" /* Use the NVIC offset register to locate the stack. */
        " ldr r0, [r0]          \n"
        " ldr r0, [r0]          \n"
        " msr msp, r0           \n" /* Set the msp back to the start of the stack. */
        " cpsie i               \n" /* Globally enable interrupts. */
        " cpsie f               \n"
        " dsb                   \n"
        " isb                   \n"
        " svc 0                 \n" /* System call to start first task. */
        " nop                   \n"
        " .ltorg                \n"
        );
}



#define portMIN_INTERRUPT_PRIORITY            ( 255UL )
#define portNVIC_PENDSV_PRI                   ( ( ( uint32_t ) portMIN_INTERRUPT_PRIORITY ) << 16UL )
#define portNVIC_SYSTICK_PRI                  ( ( ( uint32_t ) portMIN_INTERRUPT_PRIORITY ) << 24UL )

#define portNVIC_SHPR2_REG                    ( *( ( volatile uint32_t * ) 0xe000ed1c ) )
#define portNVIC_SHPR3_REG                    ( *( ( volatile uint32_t * ) 0xe000ed20 ) )

long xStartScheduler( void )
{
   
    portNVIC_SHPR3_REG |= portNVIC_PENDSV_PRI;
    portNVIC_SHPR3_REG |= portNVIC_SYSTICK_PRI;
    portNVIC_SHPR2_REG = 0;


    /* Start the first task. */
    prvPortStartFirstTask();



    return 0;
}


#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	191

void xPendSVHandler( void )
{

    __asm volatile
    (
        "   mrs r0, psp                         \n"
        "   isb                                 \n"
        "                                       \n"
        "   ldr r3, pxCurrentTCBConst           \n" /* Get the location of the current TCB. */
        "   ldr r2, [r3]                        \n"
        "                                       \n"
        "   stmdb r0!, {r4-r11}                 \n" /* Save the remaining registers. */
        "   str r0, [r2]                        \n" /* Save the new top of stack into the first member of the TCB. */
        "                                       \n"
        "   stmdb sp!, {r3, r14}                \n"
        "   mov r0, %0                          \n"
        "   msr basepri, r0                     \n"
        "   bl vTaskSwitchContext               \n"
        "   mov r0, #0                          \n"
        "   msr basepri, r0                     \n"
        "   ldmia sp!, {r3, r14}                \n"
        "                                       \n" /* Restore the context, including the critical nesting count. */
        "   ldr r1, [r3]                        \n"
        "   ldr r0, [r1]                        \n" /* The first item in pxCurrentTCB is the task top of stack. */
        "   ldmia r0!, {r4-r11}                 \n" /* Pop the registers. */
        "   msr psp, r0                         \n"
        "   isb                                 \n"
        "   bx r14                              \n"
        "                                       \n"
        "   .align 4                            \n"
        "pxCurrentTCBConst: .word pxCurrentTCB  \n"
        ::"i" ( configMAX_SYSCALL_INTERRUPT_PRIORITY )
    );
}

