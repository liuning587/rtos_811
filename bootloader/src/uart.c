/**
 * @file   : uart.c
 * Fuction : CM3 bootloader
 * By      : LiuNing
 * Date    : 2012-3-10
 */

#include "./inc/lm3s.h"
#include "./inc/cfg.h"
#include "./inc/lib.h"
#include "./inc/uart.h"

/***************************************************************************//**
*
* uart_init - uart初始化
*
* @param void
*
* @return void
*
*******************************************************************************/
void uart_init(void)
{
    uint32 ulProcRatio;

    // 使能片内外设GPIOA和UART1
    HWREG(SYSCTL_RCGC2) |= SYSCTL_RCGC2_GPIOA;
    HWREG(SYSCTL_RCGC1) |= SYSCTL_RCGC1_UART0;

    ulProcRatio = UART_BAUD_RATIO(UART_BAUDRATE);

    // 引脚备用功能选择
    HWREG(GPIO_PORTA_BASE + GPIO_O_AFSEL) |= UART_PINS;
    // 引脚数字使能
    HWREG(GPIO_PORTA_BASE + GPIO_O_DEN)   |= UART_PINS;

    // 设置波特率
    HWREG(UART_BASE + UART_O_IBRD) = ulProcRatio >> 6;
    HWREG(UART_BASE + UART_O_FBRD) = ulProcRatio & UART_FBRD_DIVFRAC_M;

    // 设置8位数据长、1位停止位、无校验、使能FIFO
    HWREG(UART_BASE + UART_O_LCRH) = UART_LCRH_WLEN_8 | UART_LCRH_FEN;

    // 使能UART、发送、接收
    HWREG(UART_BASE + UART_O_CTL) = (UART_CTL_UARTEN | UART_CTL_TXE |
                                     UART_CTL_RXE);

}

/***************************************************************************//**
*
* uart_send - 发送指定长度的数据
*
* @param [in] c 要发送的字符
*
* @return void
*
*******************************************************************************/
void uart_send(char c)
{
    // 如果发送FIFO满则等待
    while((HWREG(UART_BASE + UART_O_FR) & UART_FR_TXFF))
    {
    }

    // 发送一个字符
    HWREG(UART_BASE + UART_O_DR) = (uint32) c;

    uart_flush();
}

/***************************************************************************//**
*
* uart_flush - 等待UART发送结束
*
* @param void
*
* @return void
*
*******************************************************************************/
void uart_flush(void)
{
    // 如果发送FIFO不为空，则等待其发送完成
    while(!(HWREG(UART_BASE + UART_O_FR) & UART_FR_TXFE))
    {
    }

    // 如果UART忙于发送数据，则继续等待
    while((HWREG(UART_BASE + UART_O_FR) & UART_FR_BUSY))
    {
    }
}

/***************************************************************************//**
*
* uart_receive - 接收指定长度的数据
*
* @param [in] pData 接收缓冲
* @param [in] size  接收长度
*
* @return void
*
*******************************************************************************/
void uart_receive(uint8 *pData, uint32 size)
{
    while (size--)
    {
        // 如果FIFO为空，则等待
        while ((HWREG(UART_BASE + UART_O_FR) & UART_FR_RXFE))
        {
        }
        // 读取一个字节放入缓冲
        *pData++ = (uint8) (HWREG(UART_BASE + UART_O_DR));
    }
}

/***************************************************************************//**
*
* uart_receiveNoBlocking - 接收0~1个字符
*
* @param [in] pData 接收缓冲
*
* @return void
*
*******************************************************************************/
boolean uart_receiveNoBlocking(uint8 *pData)
{
    // 如果FIFO无数据，则直接返回
    if ((HWREG(UART_BASE + UART_O_FR) & UART_FR_RXFE))
        return false;

    // 读取一个字节放入缓冲
    *pData = (uint8) (HWREG(UART_BASE + UART_O_DR));

    return true;
}

/***************************************************************************//**
*
* uart_printf - 往串口发送一个字符串
*
* @param [in] str 字符串以'\0'结尾
*
* @return void
*
*******************************************************************************/
void uart_printf(char *str)
{
    while('\0' != *str) uart_send(*str++);
}

/*--------------------------------End of uart.c-------------------------------*/
