@****************************************************************************
@   File    : Startup.s
@   Fuction : CM3 start up code.
@   By      : LiuNing
@   Date    : 2012-3-10
@****************************************************************************

    @�ⲿ��������
    .extern  updater                        @����������  
    .extern  sysTickIntHandler              @��ʱ�ж�
	.equ     STACK_SIZE,        256         @�Ѵ�С*4
    .equ     NVIC_VTABLE,       0xe000ed08  @�ж�������
	.equ     APP_START_ADDRESS, 0x00000800  @Ӧ�ó�����ڵ�ַ
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
@ ������
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
    @ ��λ�����ӳ���
ResetISR:
    @ ��flash�е�BootLoader���Ƶ�sram�У�����ʼ��bss��
    @ flash���ݷֲ�����:
    @ 0x0003-FFFF +-----------------------------------+ 
    @             |                                   |
    @             |                                   |
    @             +-----------------------------------+
    @             | ���ݶγ�ʼ����                    |
    @             +-----------------------------------+
    @             | ֻ�����ݶ�(����)                  |
    @             +-----------------------------------+
    @             | �����                            |
    @             +-----------------------------------+
    @             | ������                            |
    @ 0x0000-0000 +-----------------------------------+
    @
    @ processorInitִ�����sram���ݷֲ�����:
    @ 0x2000-FFFF +-----------------------------------+ 
    @             |                                   |
    @             |                                   |
    @             +-----------------------------------+ -> g_pulStack + STACK_SIZE * 4(ջ��)
    @             |                                   |
    @             | ����ջ                            |
    @             |                                   |   
    @             +-----------------------------------+ -> g_pulStack(ջ�����ֵ)
    @             | bss��(��0���)                    |
    @             +-----------------------------------+
    @             | �ɱ����ݶ�(�г�ֵ��ȫ�ֱ���)      |
    @             +-----------------------------------+
    @             | ֻ�����ݶ�(����)                  |
    @             +-----------------------------------+
    @             | �����                            |
    @             +-----------------------------------+
    @             | ������                            |
    @ 0x2000-0000 +-----------------------------------+
    
    movs    r0, #0x0000     @ 0x00000000 -> r0(r0ָ��flash��ʼλ��)
    movs    r1, #0x0000     @ 0x00000000 -> r1
    movt    r1, #0x2000     @ ��0x2000����r1�ĸ߰�����(r1ָ��sram��ʼλ��)	 
    .extern _bss
    ldr     r2, =_bss       @ ��.bbs�ε���ʼ��ַ����r2��
	mov     r4, pc	        @ ����PCָ�뵽r4, ������ˮ��ԭ��pc������copy_loop��ַ
	nop						@ ��ˮ��	
copy_loop:
    ldr     r3, [r0], #4    @ ��r0��ַ����ֵ����r3��(flash -> r3)��Ȼ��r0 + 4 -> r0
    str     r3, [r1], #4    @ ��r3�����ݴ���r1��ַ��(r3 -> sram)��Ȼ��r1 + 4 -> r1
    cmp     r1, r2          @ �Ƚ�r1��r2
                            @ ���r1 < r2���������(blt copy_loop)
    it	    ne              @ ����Ƿ����
    movne   pc, r4          @ ��r1 <r2 ����ת��copy_loop��������,
                            @ Ϊ��bootloader����д�Լ����������ʹ��PC��ƫ����
	nop					    @ ��ˮ��
	nop						@ ��ˮ��
	
    @ ��0���.bss��
    movs    r0, #0x0000     @ 0 -> r0	  
    .extern _ebss
    ldr     r2, =_ebss      @ ��.bss�εĽ�����ַ����r2��
	mov     r4, pc	        @ ����PCָ�뵽r4, ������ˮ��ԭ��pc������copy_loop��ַ
	nop						@ ��ˮ��
zero_loop:
    str     r0, [r1], #4    @ ��r0������(0)����r1��ַ����Ȼ��r1 + 4 -> r1
    cmp     r1, r2          @ �Ƚ�r1��r2
                            @ ���r1 < r2���������(blt zero_loop)
    it	    ne              @ ����Ƿ����
    movne   pc, r4          @ ��r1 <r2 ����ת��copy_loop��������
	nop						@ ��ˮ��
	nop					    @ ��ˮ��
    
    @ ��������ָ��sram��
    movw    r0, #(NVIC_VTABLE & 0xffff) @ ��������ƫ�����Ĵ�����ַ����r0��   
    movt    r0, #(NVIC_VTABLE >> 16)

    movs    r1, #0x0000     @ 0x20000000 -> r1
    movt    r1, #0x2000
    
    str     r1, [r0]        @ ��r1�����ݴ���r0��ַ��(������ƫ�����Ĵ���)
    
	movs    r2, #0x0004     @ 0x00000004 -> r1
    movt    r2, #0x2000     @ ��0x2000����r1�ĸ߰�����(r1ָ��sram��ʼλ��)	 
	ldr     r1, [r2]	  
	orr     r1, r1, #0x20000000	 @�޸ĸ�λ�ж����
    str     r1, [r2]
							@ ���뿽����ram�к��޸�pcָ��
	add     r4, pc, #8      @ ��pcָ�� ��  0x20000000 
	orr     r4, r4, #0x20000000
	mov     pc, r4          @ ����������RAM������
	nop
	nop

    @ ��ʼ����
EnterBootLoader:
    bl      updater
    bl      CallApplication
    
    @ ����Ӧ�ó���
    .thumb_func
CallApplication:
    @ ���ж�����ƫ�ƼĴ���ֵ��ΪӦ�ó������ʼ��ַ
    movw    r0, #(APP_START_ADDRESS & 0xffff)    @ Ӧ�ó�����ʼ��ַ -> r0
    movw    r1, #(NVIC_VTABLE & 0xffff)          @ �ж�����ƫ�ƼĴ�����ַ -> r1
    movt    r1, #(NVIC_VTABLE >> 16)
    str     r0, [r1]                             @ r0 -> �ж�����ƫ�ƼĴ���

    @ ���ö�ջָ��
    ldr     sp, [r0]

    @ ��ת��Ӧ�ó���
    ldr     r0, [r0, #4]                         @ r0 + 4��ַ����ֵ -> r0(ָ��Ӧ�ó���ĸ�λ�жϵ�ַ)
    bx      r0                                   @ r0 -> pc����ת��r0��ַ��ִ��

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