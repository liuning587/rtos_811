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
* uart_init - uart��ʼ��
*
* @param void
*
* @return void
*
*******************************************************************************/
void uart_init(void)
{
    uint32 ulProcRatio;

    // ʹ��Ƭ������GPIOA��UART1
    HWREG(SYSCTL_RCGC2) |= SYSCTL_RCGC2_GPIOA;
    HWREG(SYSCTL_RCGC1) |= SYSCTL_RCGC1_UART0;

    ulProcRatio = UART_BAUD_RATIO(UART_BAUDRATE);

    // ���ű��ù���ѡ��
    HWREG(GPIO_PORTA_BASE + GPIO_O_AFSEL) |= UART_PINS;
    // ��������ʹ��
    HWREG(GPIO_PORTA_BASE + GPIO_O_DEN)   |= UART_PINS;

    // ���ò�����
    HWREG(UART_BASE + UART_O_IBRD) = ulProcRatio >> 6;
    HWREG(UART_BASE + UART_O_FBRD) = ulProcRatio & UART_FBRD_DIVFRAC_M;

    // ����8λ���ݳ���1λֹͣλ����У�顢ʹ��FIFO
    HWREG(UART_BASE + UART_O_LCRH) = UART_LCRH_WLEN_8 | UART_LCRH_FEN;

    // ʹ��UART�����͡�����
    HWREG(UART_BASE + UART_O_CTL) = (UART_CTL_UARTEN | UART_CTL_TXE |
                                     UART_CTL_RXE);

}

/***************************************************************************//**
*
* uart_send - ����ָ�����ȵ�����
*
* @param [in] c Ҫ���͵��ַ�
*
* @return void
*
*******************************************************************************/
void uart_send(char c)
{
    // �������FIFO����ȴ�
    while((HWREG(UART_BASE + UART_O_FR) & UART_FR_TXFF))
    {
    }

    // ����һ���ַ�
    HWREG(UART_BASE + UART_O_DR) = (uint32) c;

    uart_flush();
}

/***************************************************************************//**
*
* uart_flush - �ȴ�UART���ͽ���
*
* @param void
*
* @return void
*
*******************************************************************************/
void uart_flush(void)
{
    // �������FIFO��Ϊ�գ���ȴ��䷢�����
    while(!(HWREG(UART_BASE + UART_O_FR) & UART_FR_TXFE))
    {
    }

    // ���UARTæ�ڷ������ݣ�������ȴ�
    while((HWREG(UART_BASE + UART_O_FR) & UART_FR_BUSY))
    {
    }
}

/***************************************************************************//**
*
* uart_receive - ����ָ�����ȵ�����
*
* @param [in] pData ���ջ���
* @param [in] size  ���ճ���
*
* @return void
*
*******************************************************************************/
void uart_receive(uint8 *pData, uint32 size)
{
    while (size--)
    {
        // ���FIFOΪ�գ���ȴ�
        while ((HWREG(UART_BASE + UART_O_FR) & UART_FR_RXFE))
        {
        }
        // ��ȡһ���ֽڷ��뻺��
        *pData++ = (uint8) (HWREG(UART_BASE + UART_O_DR));
    }
}

/***************************************************************************//**
*
* uart_receiveNoBlocking - ����0~1���ַ�
*
* @param [in] pData ���ջ���
*
* @return void
*
*******************************************************************************/
boolean uart_receiveNoBlocking(uint8 *pData)
{
    // ���FIFO�����ݣ���ֱ�ӷ���
    if ((HWREG(UART_BASE + UART_O_FR) & UART_FR_RXFE))
        return false;

    // ��ȡһ���ֽڷ��뻺��
    *pData = (uint8) (HWREG(UART_BASE + UART_O_DR));

    return true;
}

/***************************************************************************//**
*
* uart_printf - �����ڷ���һ���ַ���
*
* @param [in] str �ַ�����'\0'��β
*
* @return void
*
*******************************************************************************/
void uart_printf(char *str)
{
    while('\0' != *str) uart_send(*str++);
}

/*--------------------------------End of uart.c-------------------------------*/
