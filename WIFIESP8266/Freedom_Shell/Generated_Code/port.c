/*
    FreeRTOS V7.2.0 - Copyright (C) 2012 Real Time Engineers Ltd.


    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?                                      *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************


    http://www.FreeRTOS.org - Documentation, training, latest information,
    license and contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool.

    Real Time Engineers ltd license FreeRTOS to High Integrity Systems, who sell
    the code with commercial support, indemnification, and middleware, under
    the OpenRTOS brand: http://www.OpenRTOS.com.  High Integrity Systems also
    provide a safety engineered and independently SIL3 certified version under
    the SafeRTOS brand: http://www.SafeRTOS.com.
*/

/* include inherited beans */
#include "UTIL1.h"
#include "RTOSTICKLDD1.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Constants required to manipulate the NVIC. */
#define portNVIC_INT_CTRL        ((volatile unsigned long*)0xe000ed04) /* interrupt control and state register (ICSR) */
#define portNVIC_SYSPRI3         ((volatile unsigned long*)0xe000ed20) /* system handler priority register 3 (SHPR3), used for SysTick and PendSV priority */
#define portNVIC_PENDSVSET       0x10000000 /* bit 28 in portNVIC_INT_CTRL (PENDSVSET), see http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0552a/Cihfaaha.html */
#define portNVIC_PENDSV_PRI      (((unsigned long)configKERNEL_INTERRUPT_PRIORITY)<<16) /* priority of PendableService interrupt */
#define portNVIC_SYSTICK_PRI     (((unsigned long)configKERNEL_INTERRUPT_PRIORITY)<<24) /* priority of SysTick interrupt */

/* Constants required to set up the initial stack. */
#define portINITIAL_XPSR         (0x01000000)
#define portINITIAL_EXEC_RETURN  (0xfffffffd)
static LDD_TDeviceData *RTOS_TickDevice;
/* Used to keep track of the number of nested calls to taskENTER_CRITICAL().
   This will be set to 0 prior to the first task being started. */
/* Each task maintains its own interrupt status in the critical nesting variable. */
static unsigned portBASE_TYPE uxCriticalNesting = 0xaaaaaaaa;

/*-----------------------------------------------------------*/
portSTACK_TYPE *pxPortInitialiseStack( portSTACK_TYPE * pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters ) {
  /* Simulate the stack frame as it would be created by a context switch interrupt. */
  pxTopOfStack--;
  *pxTopOfStack = portINITIAL_XPSR;   /* xPSR */
  pxTopOfStack--;
  *pxTopOfStack = (portSTACK_TYPE)pxCode;  /* PC */
  pxTopOfStack--;
  *pxTopOfStack = 0;  /* LR */

  /* Save code space by skipping register initialization. */
  pxTopOfStack -= 5;  /* R12, R3, R2 and R1. */
  *pxTopOfStack = (portSTACK_TYPE)pvParameters; /* R0 */

  pxTopOfStack -= 8;  /* R11, R10, R9, R8, R7, R6, R5 and R4. */
  return pxTopOfStack;
}
/*-----------------------------------------------------------*/
portBASE_TYPE xPortStartScheduler(void) {
  /* Make PendSV and SysTick the lowest priority interrupts. */
  /* Overwrite PendSV priority as set inside the CPU component: it needs to have the lowest priority! */
  *(portNVIC_SYSPRI3) |= portNVIC_PENDSV_PRI; /* set priority of PendSV interrupt */
  uxCriticalNesting = 0; /* Initialize the critical nesting count ready for the first task. */
  RTOS_TickDevice = RTOSTICKLDD1_Init(NULL); /* initialize the tick timer */
  /* overwrite SysTick priority is set inside the FreeRTOS component */
  *(portNVIC_SYSPRI3) |= portNVIC_SYSTICK_PRI; /* set priority of SysTick interrupt */
  /* Start the timer that generates the tick ISR. Interrupts are disabled here already. */
  (void)RTOSTICKLDD1_Enable(RTOS_TickDevice); /* start the tick timer */
  vPortStartFirstTask(); /* Start the first task. */
  /* Should not get here, unless you call vTaskEndScheduler()! */
  return pdFALSE;
}
/*-----------------------------------------------------------*/
void vPortEnterCritical(void) {
/*
 * Disable interrupts before incrementing the count of critical section nesting.
 * The nesting count is maintained so we know when interrupts should be
 * re-enabled.  Once interrupts are disabled the nesting count can be accessed
 * directly.  Each task maintains its own nesting count.
 */
  portDISABLE_INTERRUPTS();
  uxCriticalNesting++;
}
/*-----------------------------------------------------------*/
void vPortExitCritical(void) {
 /* Interrupts are disabled so we can access the nesting count directly.  If the
  * nesting is found to be 0 (no nesting) then we are leaving the critical
  * section and interrupts can be re-enabled.
  */
  uxCriticalNesting--;
  if (uxCriticalNesting == 0)  {
    portENABLE_INTERRUPTS();
  }
}
/*-----------------------------------------------------------*/
void vPortYieldFromISR(void) {
  /* Set a PendSV to request a context switch. */
  *(portNVIC_INT_CTRL) = portNVIC_PENDSVSET;
}
/*-----------------------------------------------------------*/

/* return the tick raw counter value. It is assumed that the counter register has been reset at the last tick time */
portLONG uxGetTickCounterValue(void) {
  return (portLONG)RTOSTICKLDD1_GetCounterValue(RTOS_TickDevice);
}
/*-----------------------------------------------------------*/
__attribute__ ((naked)) void vOnCounterRestart(void) {
#if FREERTOS_CPU_CORTEX_M==4 /* Cortex M4 */
  #if __OPTIMIZE_SIZE__ || __OPTIMIZE__
    /*
              RTOSTICKLDD1_Interrupt:
    249       {
    0000102c:   cpy r0,sp
    0000102e:   bic r1,r0,#0x7
    00001032:   cpy sp,r1
    00001034:   push {r0,lr}  <===
    ...
    00001046:   bl RTOSTICKLDD1_OnCounterRestart (0x90c); 0x0000090c
    ....
    */
    /* nothing needed, as SP and LR are already on the stack */
  #else
  /* Code in RTOSTICKLDD1_Interrupt():
   0:   4668            mov     r0, sp
   2:   f020 0107       bic.w   r1, r0, #7
   6:   468d            mov     sp, r1
   8:   b581            push    {r0, r7, lr}   <=============
   a:   b083            sub     sp, #12        <=============
   c:   af00            add     r7, sp, #0
  ...
  38:   f7ff fffe       bl      0 <RTOSTICKLDD1_OnCounterRestart>
  3c:   f107 070c       add.w   r7, r7, #12     <============
  40:   46bd            mov     sp, r7
  42:   e8bd 4081       ldmia.w sp!, {r0, r7, lr} <===========
  46:   4685            mov     sp, r0
  48:   4770            bx      lr
  */
  __asm volatile (
    " add sp,sp,#12   \n" /* remove space allocated in RTOSTICKLDD1_Interrupt() */
    " pop {r0,r7,lr}  \n" /* these were pushed in RTOSTICKLDD1_Interrupt() */
    " push {r4,lr}    \n"
  );
  #endif
#else /* Cortex M0+ */
  #if __OPTIMIZE_SIZE__ || __OPTIMIZE__
  /*
PE_ISR(RTOSTICKLDD1_Interrupt)
{
   0:   b508            push    {r3, lr}  <<================
    RTOSTICKLDD1_TDeviceDataPtr DeviceDataPrv = INT_SysTick__BAREBOARD_RTOS_ISRPARAM;
   2:   4b05            ldr     r3, [pc, #20]   ; (18 <RTOSTICKLDD1_Interrupt+0x18>)
  SysTick_PDD_ClearInterruptFlag(SysTick_BASE_PTR); // Clear interrupt flag
   4:   4905            ldr     r1, [pc, #20]   ; (1c <RTOSTICKLDD1_Interrupt+0x1c>)
  RTOSTICKLDD1_TDeviceDataPtr DeviceDataPrv = INT_SysTick__BAREBOARD_RTOS_ISRPARAM;
   6:   681a            ldr     r2, [r3, #0]
  SysTick_PDD_ClearInterruptFlag(SysTick_BASE_PTR); // Clear interrupt flag
   8:   4b05            ldr     r3, [pc, #20]   ; (20 <RTOSTICKLDD1_Interrupt+0x20>)
   a:   6818            ldr     r0, [r3, #0]
   c:   4001            ands    r1, r0
   e:   6019            str     r1, [r3, #0]
  RTOSTICKLDD1_OnCounterRestart(DeviceDataPrv->UserDataPtr); // Invoke OnCounterRestart event
  10:   6810            ldr     r0, [r2, #0]
  12:   f7ff fffe       bl      0 <RTOSTICKLDD1_OnCounterRestart>
   */
  /* nothing needed, as r3 and LR are already on the stack */
  #else
  /*
    //PE_ISR(RTOSTICKLDD1_Interrupt):
     0: b580            push    {r7, lr} <<===================
     2: b082            sub     sp, #8      <<====================
     4: af00            add     r7, sp, #0
    RTOSTICKLDD1_TDeviceDataPtr DeviceDataPrv = INT_SysTick__BAREBOARD_RTOS_ISRPARAM;
     6: 4b09            ldr     r3, [pc, #36]   ; (2c <RTOSTICKLDD1_Interrupt+0x2c>)
     8: 681b            ldr     r3, [r3, #0]
     a: 607b            str     r3, [r7, #4]

    SysTick_PDD_ClearInterruptFlag(SysTick_BASE_PTR); // Clear interrupt flag
     c: 4b08            ldr     r3, [pc, #32]   ; (30 <RTOSTICKLDD1_Interrupt+0x30>)
     e: 4a08            ldr     r2, [pc, #32]   ; (30 <RTOSTICKLDD1_Interrupt+0x30>)
    10: 6812            ldr     r2, [r2, #0]
    12: 1c11            adds    r1, r2, #0
    14: 4a07            ldr     r2, [pc, #28]   ; (34 <RTOSTICKLDD1_Interrupt+0x34>)
    16: 400a            ands    r2, r1
    18: 601a            str     r2, [r3, #0]
    RTOSTICKLDD1_OnCounterRestart(DeviceDataPrv->UserDataPtr); // Invoke OnCounterRestart event
    1a: 687b            ldr     r3, [r7, #4]
    1c: 681b            ldr     r3, [r3, #0]
    1e: 1c18            adds    r0, r3, #0
    20: f7ff fffe       bl      0 <RTOSTICKLDD1_OnCounterRestart>
   */
  __asm volatile (
    " add sp,#8       \n" /* remove space allocated in RTOSTICKLDD1_Interrupt() */
    " pop {r0, r7}    \n" /* restore r7 and lr into r0 and r7, which were pushed in RTOSTICKLDD1_Interrupt() */
    " push {r0, r7}   \n"
  );
  #endif
#endif
  /* If using preemption, also force a context switch. */
  #if configUSE_PREEMPTION == 1
  *(portNVIC_INT_CTRL) = portNVIC_PENDSVSET;
  #endif
  portSET_INTERRUPT_MASK();   /* disable interrupts */
  vTaskIncrementTick(); /* increment tick count, might schedule a task */
  portCLEAR_INTERRUPT_MASK(); /* enable interrupts again */
#if FREERTOS_CPU_CORTEX_M==4 /* Cortex M4 */
  #if __OPTIMIZE_SIZE__ || __OPTIMIZE__
  __asm volatile (
    " pop {r0,lr}  \n" /* start exit sequence from interrupt: sp and lr where pushed */
    " mov sp, r0   \n"
    " bx lr        \n"
    " nop          \n"
  );
  #else
  __asm volatile (
    " pop {r4,lr}  \n" /* start exit sequence from interrupt: r4 and lr where pushed above */
    " bx lr        \n"
    " nop          \n"
  );
  #endif
#else /* Cortex M0+ */
#if __OPTIMIZE_SIZE__ || __OPTIMIZE__
  __asm volatile (
    " pop {r3,pc}                \n" /* start exit sequence from interrupt: r3 and lr where pushed above */
  );
#else
  __asm volatile (
    " pop {r7,pc}                \n" /* start exit sequence from interrupt: r7 and lr where pushed above */
  );
#endif
#endif
}
/*-----------------------------------------------------------*/
void vPortStartFirstTask(void) {
#if FREERTOS_CPU_CORTEX_M==4 /* Cortex M4 */
  __asm volatile (
    " ldr r0, =0xE000ED08 \n" /* Use the NVIC offset register to locate the stack. */
    " ldr r0, [r0]        \n"
    " ldr r0, [r0]        \n"
    " msr msp, r0         \n" /* Set the msp back to the start of the stack. */
    " cpsie i             \n" /* Globally enable interrupts. */
    " svc 0               \n" /* System call to start first task. */
    " nop                 \n"
  );
#else /* Cortex M0+ */
  __asm volatile(
    " movs r0, #0x00      \n" /* Locate the top of stack. */
    " ldr r0, [r0]        \n"
    " msr msp, r0         \n" /* Set the msp back to the start of the stack. */
    " cpsie i             \n" /* Globally enable interrupts. */
    " svc 0               \n" /* System call to start first task. */
    " nop                 \n"
  );
#endif
}
/*-----------------------------------------------------------*/
__attribute__ ((naked)) void vPortSVCHandler(void) {
#if FREERTOS_CPU_CORTEX_M==4 /* Cortex M4 */
__asm volatile (
    " ldr r3, pxCurrentTCBConst2 \n" /* Restore the context. */
    " ldr r1, [r3]               \n" /* Use pxCurrentTCBConst to get the pxCurrentTCB address. */
    " ldr r0, [r1]               \n" /* The first item in pxCurrentTCB is the task top of stack. */
    /* pop the core registers */
    " ldmia r0!, {r4-r11}        \n"
    " msr psp, r0                \n"
    " mov r0, #0                 \n"
    " msr basepri, r0            \n"
    " orr r14, r14, #13          \n"
    " bx r14                     \n"
    "                            \n"
    " .align 2                   \n"
    "pxCurrentTCBConst2: .word pxCurrentTCB \n"
  );
#else /* Cortex M0+ */
  __asm volatile (
    " ldr r3, pxCurrentTCBConst2 \n" /* Restore the context. */
    " ldr r1, [r3]               \n" /* Use pxCurrentTCBConst to get the pxCurrentTCB address. */
    " ldr r0, [r1]               \n" /* The first item in pxCurrentTCB is the task top of stack. */
    " add r0, r0, #16            \n" /* Move to the high registers. */
    " ldmia r0!, {r4-r7}         \n" /* Pop the high registers. */
    " mov r8, r4                 \n"
    " mov r9, r5                 \n"
    " mov r10, r6                \n"
    " mov r11, r7                \n"
    "                            \n"
    " msr psp, r0                \n" /* Remember the new top of stack for the task. */
    "                            \n"
    " sub r0, r0, #32            \n" /* Go back for the low registers that are not automatically restored. */
    " ldmia r0!, {r4-r7}         \n" /* Pop low registers.  */
    " mov r1, r14                \n" /* OR R14 with 0x0d. */
    " movs r0, #0x0d             \n"
    " orr r1, r0                 \n"
    " bx r1                      \n"
    "                            \n"
    ".align 2                    \n"
    "pxCurrentTCBConst2: .word pxCurrentTCB \n"
  );
#endif
}
/*-----------------------------------------------------------*/
__attribute__ ((naked)) void vPortPendSVHandler(void) {
#if FREERTOS_CPU_CORTEX_M==4 /* Cortex M4 */
  __asm volatile (
    " mrs r0, psp                \n"
    " ldr  r3, pxCurrentTCBConst \n" /* Get the location of the current TCB. */
    " ldr  r2, [r3]              \n"
    " stmdb r0!, {r4-r11}        \n" /* Save the core registers. */
    " str r0, [r2]               \n" /* Save the new top of stack into the first member of the TCB. */
    " stmdb sp!, {r3, r14}       \n"
    " mov r0, %0                 \n"
    " msr basepri, r0            \n"
    " bl vTaskSwitchContext      \n"
    " mov r0, #0                 \n"
    " msr basepri, r0            \n"
    " ldmia sp!, {r3, r14}       \n"
    " ldr r1, [r3]               \n" /* The first item in pxCurrentTCB is the task top of stack. */
    " ldr r0, [r1]               \n"
    " ldmia r0!, {r4-r11}        \n" /* Pop the core registers. */
    " msr psp, r0                \n"
    " bx r14                     \n"
    "                            \n"
    " .align 2                   \n"
    "pxCurrentTCBConst: .word pxCurrentTCB  \n"
    ::"i"(configMAX_SYSCALL_INTERRUPT_PRIORITY)
  );
#else /* Cortex M0+ */
  __asm volatile (
    " mrs r0, psp                \n"
    "                            \n"
    " ldr r3, pxCurrentTCBConst  \n" /* Get the location of the current TCB. */
    " ldr r2, [r3]   \n"
    "                            \n"
    " sub r0, r0, #32            \n" /* Make space for the remaining low registers. */
    " str r0, [r2]               \n" /* Save the new top of stack. */
    " stmia r0!, {r4-r7}         \n" /* Store the low registers that are not saved automatically. */
    " mov r4, r8                 \n" /* Store the high registers. */
    " mov r5, r9                 \n"
    " mov r6, r10                \n"
    " mov r7, r11                \n"
    " stmia r0!, {r4-r7}         \n"
    "                            \n"
    " push {r3, r14}             \n"
    " cpsid i                    \n"
    " bl vTaskSwitchContext      \n"
    " cpsie i                    \n"
    " pop {r2, r3}               \n" /* lr goes in r3. r2 now holds tcb pointer. */
    "                            \n"
    " ldr r1, [r2]               \n"
    " ldr r0, [r1]               \n" /* The first item in pxCurrentTCB is the task top of stack. */
    " add r0, r0, #16            \n" /* Move to the high registers. */
    " ldmia r0!, {r4-r7}         \n" /* Pop the high registers. */
    " mov r8, r4                 \n"
    " mov r9, r5                 \n"
    " mov r10, r6                \n"
    " mov r11, r7                \n"
    "                            \n"
    " msr psp, r0                \n" /* Remember the new top of stack for the task. */
    "                            \n"
    " sub r0, r0, #32            \n" /* Go back for the low registers that are not automatically restored. */
    " ldmia r0!, {r4-r7}         \n" /* Pop low registers.  */
    "                            \n"
    " bx r3                      \n"
    "                            \n"
    ".align 2                    \n"
    "pxCurrentTCBConst: .word pxCurrentTCB"
  );
#endif
}

