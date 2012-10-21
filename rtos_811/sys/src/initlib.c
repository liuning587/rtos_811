//*****************************************************************************
//
// startup_gcc.c - Startup code for use with GNU tools.
//
// Copyright (c) 2006-2010 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 6594 of the EK-LM3S1968 Firmware Package.
//
//*****************************************************************************
#include <types.h>
#include <ucos_ii.h>
#include <intlib.h>
#include <cpu_os.h>
//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
void ResetISR(void);
static void NmiSR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);
static void dummy(void);
static void intHandler(uint32_t int_num);

/*FOR INTLIB*/
#define INT_COUNT 59
INT_RTN intRtnTbl[INT_COUNT - 15];                     /* 中断实现函数入口表 */
uint32_t intcount = INT_COUNT;                           /* 支持的中断个数*/

#define ROUTINE(n) routine##n
/* 其它中断 */
void    routine16   (void)  { intHandler(   16  ); }
void    routine17   (void)  { intHandler(   17  ); }
void    routine18   (void)  { intHandler(   18  ); }
void    routine19   (void)  { intHandler(   19  ); }
void    routine20   (void)  { intHandler(   20  ); }
void    routine21   (void)  { intHandler(   21  ); }
void    routine22   (void)  { intHandler(   22  ); }
void    routine23   (void)  { intHandler(   23  ); }
void    routine24   (void)  { intHandler(   24  ); }
void    routine25   (void)  { intHandler(   25  ); }
void    routine26   (void)  { intHandler(   26  ); }
void    routine27   (void)  { intHandler(   27  ); }
void    routine28   (void)  { intHandler(   28  ); }
void    routine29   (void)  { intHandler(   29  ); }
void    routine30   (void)  { intHandler(   30  ); }
void    routine31   (void)  { intHandler(   31  ); }
void    routine32   (void)  { intHandler(   32  ); }
void    routine33   (void)  { intHandler(   33  ); }
void    routine34   (void)  { intHandler(   34  ); }
void    routine35   (void)  { intHandler(   35  ); }
void    routine36   (void)  { intHandler(   36  ); }
void    routine37   (void)  { intHandler(   37  ); }
void    routine38   (void)  { intHandler(   38  ); }
void    routine39   (void)  { intHandler(   39  ); }
void    routine40   (void)  { intHandler(   40  ); }
void    routine41   (void)  { intHandler(   41  ); }
void    routine42   (void)  { intHandler(   42  ); }
void    routine43   (void)  { intHandler(   43  ); }
void    routine44   (void)  { intHandler(   44  ); }
void    routine45   (void)  { intHandler(   45  ); }
void    routine46   (void)  { intHandler(   46  ); }
void    routine47   (void)  { intHandler(   47  ); }
void    routine48   (void)  { intHandler(   48  ); }
void    routine49   (void)  { intHandler(   49  ); }
void    routine50   (void)  { intHandler(   50  ); }
void    routine51   (void)  { intHandler(   51  ); }
void    routine52   (void)  { intHandler(   52  ); }
void    routine53   (void)  { intHandler(   53  ); }
void    routine54   (void)  { intHandler(   54  ); }
void    routine55   (void)  { intHandler(   55  ); }
void    routine56   (void)  { intHandler(   56  ); }
void    routine57   (void)  { intHandler(   57  ); }
void    routine58   (void)  { intHandler(   58  ); }
void    routine59   (void)  { intHandler(   59  ); }

//*****************************************************************************
//
// The entry point for the application.
//
//*****************************************************************************
extern int main(void);
extern void  EXC_ENTER_HARD_FAULT(void);
extern void  EXC_ENTER_MEM_FAULT(void);
extern void  EXC_ENTER_BUS_FAULT(void);
extern void  EXC_ENTER_USAGE_FAULT(void);
//*****************************************************************************
//
// Reserve space for the system stack.
//
//*****************************************************************************
extern unsigned long cstack_top; /* Defined by the linker */
//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
    (void (*)(void))(&cstack_top),
                                            // The initial stack pointer
    ResetISR,                               // The reset handler
    NmiSR,                                  // The NMI handler
#if 0
    FaultISR,                               // The hard fault handler
    IntDefaultHandler,                      // The MPU fault handler
    IntDefaultHandler,                      // The bus fault handler
    IntDefaultHandler,                      // The usage fault handler
#endif
    EXC_ENTER_HARD_FAULT,
    EXC_ENTER_MEM_FAULT,
    EXC_ENTER_BUS_FAULT,
    EXC_ENTER_USAGE_FAULT,
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    IntDefaultHandler,                      // SVCall handler
    IntDefaultHandler,                      // Debug monitor handler
    0,                                      // Reserved
    OS_CPU_PendSVHandler,                   // The PendSV handler
    OS_CPU_SysTickHandler,                  // The SysTick handler
    ROUTINE(16 ),                      // GPIO Port A
    ROUTINE(17 ),                      // GPIO Port B
    ROUTINE(18 ),                      // GPIO Port C
    ROUTINE(19 ),                      // GPIO Port D
    ROUTINE(20 ),                      // GPIO Port E
    ROUTINE(21 ),                      // UART0 Rx and Tx
    ROUTINE(22 ),                      // UART1 Rx and Tx
    ROUTINE(23 ),                      // SSI0 Rx and Tx
    ROUTINE(24 ),                      // I2C0 Master and Slave
    ROUTINE(25 ),                      // PWM Fault
    ROUTINE(26 ),                      // PWM Generator 0
    ROUTINE(27 ),                      // PWM Generator 1
    ROUTINE(28 ),                      // PWM Generator 2
    ROUTINE(29 ),                      // Quadrature Encoder 0
    ROUTINE(30 ),                      // ADC Sequence 0
    ROUTINE(31 ),                      // ADC Sequence 1
    ROUTINE(32 ),                      // ADC Sequence 2
    ROUTINE(33 ),                      // ADC Sequence 3
    ROUTINE(34 ),                      // Watchdog timer
    ROUTINE(35 ),                      // Timer 0 subtimer A
    ROUTINE(36 ),                      // Timer 0 subtimer B
    ROUTINE(37 ),                      // Timer 1 subtimer A
    ROUTINE(38 ),                      // Timer 1 subtimer B
    ROUTINE(39 ),                      // Timer 2 subtimer A
    ROUTINE(40 ),                      // Timer 2 subtimer B
    ROUTINE(41 ),                      // Analog Comparator 0
    ROUTINE(42 ),                      // Analog Comparator 1
    ROUTINE(43 ),                      // Analog Comparator 2
    ROUTINE(44 ),                      // System Control (PLL, OSC, BO)
    ROUTINE(45 ),                      // FLASH Control
    ROUTINE(46 ),                      // GPIO Port F
    ROUTINE(47 ),                      // GPIO Port G
    ROUTINE(48 ),                      // GPIO Port H
    ROUTINE(49 ),                      // UART2 Rx and Tx
    ROUTINE(50 ),                      // SSI1 Rx and Tx
    ROUTINE(51 ),                      // Timer 3 subtimer A
    ROUTINE(52 ),                      // Timer 3 subtimer B
    ROUTINE(53 ),                      // I2C1 Master and Slave
    ROUTINE(54 ),                      // Quadrature Encoder 1
    ROUTINE(55 ),                      // CAN0
    ROUTINE(56 ),                      // CAN1
    ROUTINE(57 ),                      // CAN2
    ROUTINE(58 ),                      // Ethernet
    ROUTINE(59 )                       // Hibernate
};
/*******************************************************************************
*
* intConnect - 关联ROUTINE
*
* INPUTS:
*   int_num   : 中断号(16 ~ intcount)
*   routine   : 中断服务
*   parameter : 中断参数
*
* RETURNS:
*   status_t: 成功-OK, 失败-ERROR
*
*******************************************************************************/
extern status_t intConnect(uint32_t int_num, VOIDFUNCPTR routine, uint32_t parameter) {
    if ((int_num < 16) || (int_num > intcount))
        return ERROR;

    intRtnTbl[int_num - 16].routine   = routine;
    intRtnTbl[int_num - 16].parameter = parameter;

    return OK;
}

/*******************************************************************************
*
* intDisconnect - 注销中断ROUTINE
*
* INPUTS:
*   int_num: 中断号(16 ~ intcount)
*
* RETURNS:
*   status_t: 成功-OK, 失败-ERROR
*
*******************************************************************************/
extern status_t intDisconnect(uint32_t int_num) {
    if ((int_num < 16) || (int_num > intcount))
        return ERROR;

    intRtnTbl[int_num - 16].routine = dummy;
    intRtnTbl[int_num - 16].parameter = NULL;

    return OK;
}

/*******************************************************************************
*
* intPrioSet - 设置中断优先级
*
* INPUTS:
*   int_num   : 中断号(16 ~ intcount)
*   prio : 中断优先级
*
* RETURNS:
*   status_t: 成功-OK, 失败-ERROR
*
*******************************************************************************/
extern status_t intPrioSet(uint32_t int_num, uint8_t prio) {
    if ((int_num < 16) || (int_num > intcount))
        return ERROR;

    CPU_IntSrcPrioSet(int_num, prio);

    return OK;
}

/*******************************************************************************
*
* intEnable - 使能指定的中断
*
* INPUTS:
*   int_num   : 中断号(16 ~ intcount)
*
* RETURNS:
*   status_t: 成功-OK, 失败-ERROR
*
*******************************************************************************/
extern status_t intEnable(uint32_t int_num) {
    if ((int_num < 16) || (int_num > intcount))
        return ERROR;

    CPU_IntSrcEn(int_num);

    return OK;
}

/*******************************************************************************
*
* intDisable - 禁用指定的中断
*
* INPUTS:
*   int_num   : 中断号(16 ~ intcount - 1)
*
* RETURNS:
*   status_t: 成功-OK, 失败-ERROR
*
*******************************************************************************/
extern status_t intDisable(uint32_t int_num) {
    if ((int_num < 16) || (int_num > intcount - 1))
        return ERROR;

    CPU_IntSrcDis(int_num);

    return OK;
}

/*******************************************************************************
*
* intLock - 关中断
*
* INPUTS:
*   void
*
* RETURNS:
*   void
*
*******************************************************************************/
extern void intLock(void) {
     __asm(
           "CPSID   I\n"
           "CPSID   I\n"
          );
}

/*******************************************************************************
*
* intUnlock - 开中断
*
* INPUTS:
*   void
*
* RETURNS:
*   void
*
*******************************************************************************/
extern void intUnlock(void) {
     __asm(
           "CPSIE   I\n"
           "BX      LR\n"
          );
}

/*******************************************************************************
*
* dummy -  空闲中断入口
*
* INPUTS:
*   void
*
* RETURNS:
*   void
*
*******************************************************************************/
static void dummy(void) {
    return;
}

/*******************************************************************************
*
* intHandler -   根据传入的中断号找到并调用相应的ISR
*
* INPUTS:
*   id : 中断号
*
* RETURNS:
*   void
*
*******************************************************************************/
static void intHandler(uint32_t int_num)
{
    INT_RTN int_rtn = intRtnTbl[int_num - 16];

    OSIntEnter();

    if (NULL != int_rtn.parameter)
        int_rtn.routine(int_rtn.parameter);
    else
        int_rtn.routine();

    OSIntExit();
}
/*******************************************************************************
*
* intLibInit - 中断初始化
*
* INPUTS:
*   void
*
* RETURNS:
*   void
*
*******************************************************************************/
extern void intLibInit(void) {
    uint32_t  int_num;

    // 初始化中断表
    for (int_num = 16; int_num < intcount; int_num++) {
        intConnect(int_num, dummy, NULL);
    }

    // 使能BusFault、memFault、usgFault
    CPU_REG_NVIC_SHCSR |= CPU_REG_NVIC_SHCSR_BUSFAULTENA | CPU_REG_NVIC_SHCSR_MEMFAULTENA | CPU_REG_NVIC_SHCSR_USGFAULTENA;
}
//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
//extern unsigned long _bss;
//extern unsigned long _ebss;

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
void
ResetISR(void)
{
    unsigned long *pulSrc, *pulDest;

    //
    // Copy the data segment initializers from flash to SRAM.
    //
    pulSrc = &_etext;
    for(pulDest = &_data; pulDest < &_edata; )
    {
        *pulDest++ = *pulSrc++;
    }

    //
    // Zero fill the bss segment.
    //
    __asm("    ldr     r0, =_bss\n"
          "    ldr     r1, =_ebss\n"
          "    mov     r2, #0\n"
          "    .thumb_func\n"
          "zero_loop:\n"
          "        cmp     r0, r1\n"
          "        it      lt\n"
          "        strlt   r2, [r0], #4\n"
          "        blt     zero_loop");

    //
    // Call the application's entry point.
    //
    main();
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
static void
NmiSR(void)
{
    //
    // Enter an infinite loop.
    //
    while(1)
    {
    }
}
#if 0
//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
FaultISR(void)
{
    //
    // Enter an infinite loop.
    //
    while(1)
    {
    }
}
#endif
//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
IntDefaultHandler(void)
{
    //
    // Go into an infinite loop.
    //
    while(1)
    {
    }
}
