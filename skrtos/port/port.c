
#include "port.h"
#include "task.h"


Error_help(port)


uint32_t * Init_Stack(  uint32_t * topStack,
                        void * const pvParameters,
                            Taskfunction function_t )
{


     /*the regiters is placed in the end of array.we need 16 word to place them!
     *
     * *The registers in the structure correspond to the placement
     of 16 register parameters from low addresses to high addresses
     *
     * */


    Stack_register *Stack;
    topStack -= 16;
    Stack = (Stack_register *)topStack;

    Stack->xPSR = Init_XPSR;                                   
    
    Stack->PC = ( ( uint32_t ) function_t ) & ThumbFun_adderss; 
    
    Stack->LR = ( uint32_t ) LR_error;             
                                                     
    Stack->r0 = ( uint32_t ) NULL;

    return topStack;

}




void __attribute__( ( naked ) ) xStartFirstTask( void )
{
    __asm volatile (
            " ldr r0, =0xE000ED08 	\n"/* Use the NVIC offset register to locate the stack. */
            " ldr r0, [r0] 			\n"
            " ldr r0, [r0] 			\n"
            " msr msp, r0			\n"/* Set the msp back to the start of the stack. */
            " cpsie i				\n"/* Globally enable interrupts. */
            " cpsie f				\n"
            " dsb					\n"
            " isb					\n"
            " svc 0					\n"/* System call to start first task. */
            " nop					\n"
            " .ltorg				\n"
            );
}




void __attribute__( ( naked ) )  xSVCHandler( void )
{
    __asm volatile (
            "	ldr	r3, pxCurrentTCBConst2		\n"/* Restore the context. */
            "	ldr r1, [r3]					\n"/* Use pxCurrentTCBConst to get the pxCurrentTCB address. */
            "	ldr r0, [r1]					\n"/* The first item in pxCurrentTCB is the task top of stack. */
            "	ldmia r0!, {r4-r11}				\n"/* Pop the registers that are not automatically saved on exception entry and the critical nesting count. */
            "	msr psp, r0						\n"/* Restore the task stack pointer. */
            "	isb								\n"
            "	mov r0, #0 						\n"
            "	msr	basepri, r0					\n"
            "	orr r14, #0xd					\n"
            "	bx r14							\n"
            "									\n"
            "	.align 4						\n"
            "pxCurrentTCBConst2: .word CurrentTCB				\n"
            );
}




void __attribute__((always_inline)) contextSwitchTriger(void) {
    __asm volatile (
            "LDR R0, =0xE000ED04\n"
            "MOV R1, #1\n"
            "LSL R1, R1, #28\n"
            "STR R1, [R0]\n"
            "dsb \n"
            "isb \n"
            :
            :
            : "memory"
            );
}



__attribute__( ( naked ) ) void xPendSVHandler( void )
{
    __asm volatile
            (
            "	mrs r0, psp							\n"
            "	isb									\n"
            "										\n"
            "	ldr	r3, pxCurrentTCBConst			\n"
            "	ldr	r2, [r3]						\n"
            "										\n"
            "	stmdb r0!, {r4-r11}					\n"
            "	str r0, [r2]						\n"
            "										\n"
            "	stmdb sp!, {r3, r14}				\n"
            "	mov r0, %0							\n"
            "	msr basepri, r0						\n"
            "   dsb                                 \n"
            "   isb                                 \n"
            "	bl vTaskSwitchContext				\n"
            "	mov r0, #0							\n"
            "	msr basepri, r0						\n"
            "	ldmia sp!, {r3, r14}				\n"
            "										\n"
            "	ldr r1, [r3]						\n"
            "	ldr r0, [r1]						\n"
            "	ldmia r0!, {r4-r11}					\n"
            "	msr psp, r0							\n"
            "	isb									\n"
            "	bx r14								\n"
            "	nop									\n"
            "	.align 4							\n"
            "pxCurrentTCBConst: .word CurrentTCB	\n"
            ::"i" ( configShieldInterPriority )
            );
}


void SystickInit( void )
{
    Systick_struct *Systick;
    Systick = (Systick_struct *)portNVIC_SYSTICK_ADDRESS;
    /* Stop and clear the SysTick. */
    Systick->CTRL = 0UL;
    Systick->VAL = 0UL;

    /* Configure SysTick to interrupt at the requested rate. */
    Systick->LOAD = ( configSysTickCLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
    Systick->CTRL = ( portNVIC_SYSTICK_CLK_BIT | portNVIC_SYSTICK_INT_BIT | portNVIC_SYSTICK_ENABLE_BIT );
}


#define portNVIC_SHPR2_REG                    ( *( ( volatile uint32_t * ) 0xe000ed1c ) )
long xStartScheduler( void )
{

    ( *( ( volatile uint32_t * ) 0xe000ed20 ) ) |= ( ( ( uint32_t ) 255UL ) << 16UL );
    ( *( ( volatile uint32_t * ) 0xe000ed20 ) ) |= ( ( ( uint32_t ) 255UL ) << 24UL );
    portNVIC_SHPR2_REG = 0;// the svc must run first!So its priority must highest!

    SystickInit();


    /* Start the first task. */
    xStartFirstTask();


    vTaskSwitchContext();
    port_Error();


    return 0;
}


__attribute__((always_inline)) inline uint32_t xEnterCritical( void )
{
    uint32_t xReturn;

    __asm volatile(
            " cpsid i               \n"
            " mrs %0, basepri       \n"
            " msr basepri, %1       \n"
            " dsb                   \n"
            " isb                   \n"
            " cpsie i               \n"
            : "=r" (xReturn)
            : "r" (configShieldInterPriority)
            : "memory"
            );

    return xReturn;
}

__attribute__((always_inline)) inline void xEixtCritical( uint32_t xReturn )
{
    __asm volatile(
            " cpsid i               \n"
            " msr basepri, %0       \n"
            " dsb                   \n"
            " isb                   \n"
            " cpsie i               \n"
            :: "r" (xReturn)
            : "memory"
            );
}























