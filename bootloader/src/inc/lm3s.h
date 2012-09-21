/**
 * @file   : lm3s.h
 * Fuction : lm3s ¼Ä´æÆ÷
 * By      : LiuNing
 * Date    : 2012-3-10
 */

#ifndef _LM3S_H_
#define _LM3S_H_

// types
#define HWREG(x)                (*((volatile unsigned long *)(x)))
// gpio
#define GPIO_PORTA_BASE         0x40004000  // GPIO Port A
#define GPIO_O_DATA             0x00000000  // GPIO Data
#define GPIO_O_DIR              0x00000400  // GPIO Direction
#define GPIO_O_AFSEL            0x00000420  // GPIO Alternate Function Select
#define GPIO_O_DR2R             0x00000500  // GPIO 2-mA Drive Select
#define GPIO_O_DR4R             0x00000504  // GPIO 4-mA Drive Select
#define GPIO_O_DR8R             0x00000508  // GPIO 8-mA Drive Select
#define GPIO_O_ODR              0x0000050C  // GPIO Open Drain Select
#define GPIO_O_PUR              0x00000510  // GPIO Pull-Up Select
#define GPIO_O_PDR              0x00000514  // GPIO Pull-Down Select
#define GPIO_O_SLR              0x00000518  // GPIO Slew Rate Control Select
#define GPIO_O_DEN              0x0000051C  // GPIO Digital Enable
#define GPIO_O_AMSEL            0x00000528  // GPIO Analog Mode Select
// nvic
#define NVIC_VTABLE             0xE000ED08  // Vector Table Offset
#define NVIC_APINT              0xE000ED0C  // Application Interrupt and Reset
#define NVIC_ST_RELOAD          0xE000E014  // SysTick Reload Value Register
#define NVIC_ST_CTRL            0xE000E010  // SysTick Control and Status
#define NVIC_APINT_VECTKEY_M    0xFFFF0000  // Register Key
#define NVIC_APINT_VECTKEY      0x05FA0000  // Vector key
#define NVIC_APINT_SYSRESETREQ  0x00000004  // System Reset Request
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt Enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Enable
// memmap
#define SSI0_BASE               0x40008000  // SSI0
#define GPIO_PORTD_BASE         0x40007000  // GPIO Port D
#define GPIO_PORTF_BASE         0x40025000  // GPIO Port F
#define UART0_BASE              0x4000C000  // UART0
// flash
#define FLASH_FMA               0x400FD000  // Flash Memory Address
#define FLASH_FMD               0x400FD004  // Flash Memory Data
#define FLASH_FMC               0x400FD008  // Flash Memory Control
#define FLASH_FCRIS             0x400FD00C  // Flash Controller Raw Interrupt
#define FLASH_FMC_WRKEY         0xA4420000  // FLASH write key
#define FLASH_FMC_ERASE         0x00000002  // Erase a Page of Flash Memory
#define FLASH_FMC_WRITE         0x00000001  // Write a Word into Flash Memory
#define FLASH_FCMISC            0x400FD014  // Flash Controller Masked
#define FLASH_FCMISC_AMISC      0x00000001  // Access Masked Interrupt Status
#define FLASH_FCRIS_ARIS        0x00000001  // Access Raw Interrupt Status
#define FLASH_USECRL            0x400FE140  // USec Reload
// sysctl
#define SYSCTL_DC0              0x400FE008  // Device Capabilities 0
#define SYSCTL_DC0_FLASHSZ_M    0x0000FFFF  // Flash Size
#define SYSCTL_RCGC1            0x400FE104  // Run Mode Clock Gating Control
#define SYSCTL_RCGC2            0x400FE108  // Run Mode Clock Gating Control
#define SYSCTL_RCGC1_SSI0       0x00000010  // SSI0 Clock Gating Control
#define SYSCTL_RCGC2_GPIOA      0x00000001  // Port A Clock Gating Control
#define SYSCTL_RCC              0x400FE060  // Run-Mode Clock Configuration
#define SYSCTL_RCC_MOSCDIS      0x00000001  // Main Oscillator Disable
#define SYSCTL_RCC_OSCSRC_MAIN  0x00000000  // MOSC
#define SYSCTL_RCC_OSCSRC_M     0x00000030  // Oscillator Source
#define SYSCTL_RCGC2_GPIOD      0x00000008  // Port D Clock Gating
#define SYSCTL_RCGC2_GPIOF      0x00000020  // Port F Clock Gating Control
#define SYSCTL_RCGC1_UART0      0x00000001  // UART0 Clock Gating Control
// ssi
#define SSI_O_CR0               0x00000000  // SSI Control 0
#define SSI_O_CPSR              0x00000010  // SSI Clock Prescale
#define SSI_O_CR1               0x00000004  // SSI Control 1
#define SSI_CR1_SSE             0x00000002  // SSI Synchronous Serial Port
#define SSI_O_SR                0x0000000C  // SSI Status
#define SSI_SR_TNF              0x00000002  // SSI Transmit FIFO Not Full
#define SSI_O_DR                0x00000008  // SSI Data
#define SSI_SR_RNE              0x00000004  // SSI Receive FIFO Not Empty
#define SSI_SR_TFE              0x00000001  // SSI Transmit FIFO Empty
// uart
#define UART_O_IBRD             0x00000024  // UART Integer Baud-Rate Divisor
#define UART_O_FBRD             0x00000028  // UART Fractional Baud-Rate
#define UART_FBRD_DIVFRAC_M     0x0000003F  // Fractional Baud-Rate Divisor
#define UART_O_LCRH             0x0000002C  // UART Line Control
#define UART_LCRH_WLEN_8        0x00000060  // 8 bits
#define UART_LCRH_EPS           0x00000004  // UART Even Parity Select
#define UART_LCRH_PEN           0x00000002  // UART Parity Enable
#define UART_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART_CTL_UARTEN         0x00000001  // UART Enable
#define UART_O_CTL              0x00000030  // UART Control
#define UART_CTL_TXE            0x00000100  // UART Transmit Enable
#define UART_CTL_RXE            0x00000200  // UART Receive Enable
#define UART_O_FR               0x00000018  // UART Flag
#define UART_O_DR               0x00000000  // UART Data
#define UART_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART_FR_TXFE            0x00000080  // UART Transmit FIFO Empty
#define UART_FR_BUSY            0x00000008  // UART Busy
#define UART_FR_RXFE            0x00000010  // UART Receive FIFO Empty

#endif /* _LM3S1911_H_ */

