@****************************************************************************
@   File    : Startup.s
@   Fuction : CM3 start up code.
@   By      : LiuNing
@   Date    : 2012-3-10
@****************************************************************************

    @外部函数声明
    .extern  updater                        @升级主程序  
    .extern  sysTickIntHandler              @定时中断
	.equ     STACK_SIZE,        256         @堆大小*4
    .equ     NVIC_VTABLE,       0xe000ed08  @中断向量表
	.equ     APP_START_ADDRESS, 0x00000800  @应用程序入口地址
@*****************************************************************************
@
@ Put the assembler into the correct configuration.
@
@*****************************************************************************
    .syntax unified
    .thumb

@*****************************************************************************
@
@ The stack gets placed into the zero-init section.
@
@*****************************************************************************
    .bss

@*****************************************************************************
@
@ Allocate storage for the stack.
@
@*****************************************************************************
g_pulStack:
    .space  STACK_SIZE * 4

@*****************************************************************************
@
@ This portion of the file goes into the isr_vector section.
@
@*****************************************************************************
    .section .isr_vector

@*****************************************************************************
@
@ 向量表
@
@*****************************************************************************
Vectors:
    .word   g_pulStack + (STACK_SIZE * 4)   @ Offset 00: Initial stack pointer
    .word   ResetISR - 0x20000000           @ Offset 04: Reset handler
    .word   IntDefaultHandler               @ Offset 08: NMI handler
    .word   IntDefaultHandler               @ Offset 0C: Hard fault handler
    .word   IntDefaultHandler               @ Offset 10: MPU fault handler
    .word   IntDefaultHandler               @ Offset 14: Bus fault handler
    .word   IntDefaultHandler               @ Offset 18: Usage fault handler
    .word   0                               @ Offset 1C: Reserved
    .word   0                               @ Offset 20: Reserved
    .word   0                               @ Offset 24: Reserved
    .word   0                               @ Offset 28: Reserved
    .word   IntDefaultHandler               @ Offset 2C: SVCall handler
    .word   IntDefaultHandler               @ Offset 30: Debug monitor handler
    .word   0                               @ Offset 34: Reserved
    .word   IntDefaultHandler               @ Offset 38: PendSV handler
    .word   sysTickIntHandler               @ Offset 3C: SysTick handler
    .word   IntDefaultHandler               @ Offset 40: GPIO port A handler
@*****************************************************************************
@
@ The reset handler, which gets called when the processor starts.
@
@*****************************************************************************
	.text
    .globl  ResetISR
    .thumb_func
    @ 复位处理子程序
ResetISR:
    @ 将flash中的BootLoader复制到sram中，并初始化bss段
    @ flash内容分布如下:
    @ 0x0003-FFFF +-----------------------------------+ 
    @             |                                   |
    @             |                                   |
    @             +-----------------------------------+
    @             | 数据段初始化区                    |
    @             +-----------------------------------+
    @             | 只读数据段(常量)                  |
    @             +-----------------------------------+
    @             | 代码段                            |
    @             +-----------------------------------+
    @             | 向量表                            |
    @ 0x0000-0000 +-----------------------------------+
    @
    @ processorInit执行完后，sram内容分布如下:
    @ 0x2000-FFFF +-----------------------------------+ 
    @             |                                   |
    @             |                                   |
    @             +-----------------------------------+ -> g_pulStack + STACK_SIZE * 4(栈底)
    @             |                                   |
    @             | 主堆栈                            |
    @             |                                   |   
    @             +-----------------------------------+ -> g_pulStack(栈顶最大值)
    @             | bss段(用0填充)                    |
    @             +-----------------------------------+
    @             | 可变数据段(有初值的全局变量)      |
    @             +-----------------------------------+
    @             | 只读数据段(常量)                  |
    @             +-----------------------------------+
    @             | 代码段                            |
    @             +-----------------------------------+
    @             | 向量表                            |
    @ 0x2000-0000 +-----------------------------------+
    
    movs    r0, #0x0000     @ 0x00000000 -> r0(r0指向flash起始位置)
    movs    r1, #0x0000     @ 0x00000000 -> r1
    movt    r1, #0x2000     @ 将0x2000存入r1的高半字中(r1指向sram起始位置)	 
    .extern _bss
    ldr     r2, =_bss       @ 将.bbs段的起始地址存入r2中
	mov     r4, pc	        @ 保存PC指针到r4, 由于流水线原因pc将等于copy_loop地址
	nop						@ 流水线	
copy_loop:
    ldr     r3, [r0], #4    @ 将r0地址处的值载入r3中(flash -> r3)，然后r0 + 4 -> r0
    str     r3, [r1], #4    @ 将r3的内容存入r1地址处(r3 -> sram)，然后r1 + 4 -> r1
    cmp     r1, r2          @ 比较r1、r2
                            @ 如果r1 < r2则继续复制(blt copy_loop)
    it	    ne              @ 检测是否不相等
    movne   pc, r4          @ 若r1 <r2 则跳转至copy_loop继续复制,
                            @ 为了bootloader能烧写自己，这里必须使用PC的偏移量
	nop					    @ 流水线
	nop						@ 流水线
	
    @ 用0填充.bss段
    movs    r0, #0x0000     @ 0 -> r0	  
    .extern _ebss
    ldr     r2, =_ebss      @ 将.bss段的结束地址存入r2中
	mov     r4, pc	        @ 保存PC指针到r4, 由于流水线原因pc将等于copy_loop地址
	nop						@ 流水线
zero_loop:
    str     r0, [r1], #4    @ 将r0的内容(0)存入r1地址处，然后r1 + 4 -> r1
    cmp     r1, r2          @ 比较r1、r2
                            @ 如果r1 < r2则继续复制(blt zero_loop)
    it	    ne              @ 检测是否不相等
    movne   pc, r4          @ 若r1 <r2 则跳转至copy_loop继续复制
	nop						@ 流水线
	nop					    @ 流水线
    
    @ 将向量表指向到sram中
    movw    r0, #(NVIC_VTABLE & 0xffff) @ 将向量表偏移量寄存器地址存入r0中   
    movt    r0, #(NVIC_VTABLE >> 16)

    movs    r1, #0x0000     @ 0x20000000 -> r1
    movt    r1, #0x2000
    
    str     r1, [r0]        @ 将r1的内容存入r0地址处(向量表偏移量寄存器)
    
	movs    r2, #0x0004     @ 0x00000004 -> r1
    movt    r2, #0x2000     @ 将0x2000存入r1的高半字中(r1指向sram起始位置)	 
	ldr     r1, [r2]	  
	orr     r1, r1, #0x20000000	 @修改复位中断向表
    str     r1, [r2]
							@ 代码拷贝至ram中后，修改pc指针
	add     r4, pc, #8      @ 将pc指针 或  0x20000000 
	orr     r4, r4, #0x20000000
	mov     pc, r4          @ 这里程序进入RAM中运行
	nop
	nop

    @ 开始升级
EnterBootLoader:
    bl      updater
    bl      CallApplication
    
    @ 进入应用程序
    .thumb_func
CallApplication:
    @ 将中断向量偏移寄存器值设为应用程序的起始地址
    movw    r0, #(APP_START_ADDRESS & 0xffff)    @ 应用程序起始地址 -> r0
    movw    r1, #(NVIC_VTABLE & 0xffff)          @ 中断向量偏移寄存器地址 -> r1
    movt    r1, #(NVIC_VTABLE >> 16)
    str     r0, [r1]                             @ r0 -> 中断向量偏移寄存器

    @ 设置堆栈指针
    ldr     sp, [r0]

    @ 跳转到应用程序
    ldr     r0, [r0, #4]                         @ r0 + 4地址处的值 -> r0(指向到应用程序的复位中断地址)
    bx      r0                                   @ r0 -> pc，跳转到r0地址处执行

@******************************************************************************
@
@ This is the code that gets called when the processor receives an unexpected
@ interrupt.  This simply enters an infinite loop, preserving the system state
@ for examination by a debugger.
@
@******************************************************************************
    .thumb_func
IntDefaultHandler:
        b       .

@*****************************************************************************
@
@ This is the end of the file.
@
@*****************************************************************************
    .end
