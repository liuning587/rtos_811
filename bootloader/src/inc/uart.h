/**
 * @file   : uart.c
 * Fuction : CM3 bootloader
 * By      : LiuNing
 * Date    : 2012-3-10
 */

#ifndef __UART_H__
#define __UART_H__

#include "cfg.h"

#define UART_BAUD_RATIO(ulBaud) ((((CRYSTAL_FREQ * 8) / ulBaud) + 1) / 2)
#define UART_BASE               UART0_BASE
#define UART_TX                 (1 << 0)
#define UART_RX                 (1 << 1)
#define UART_PINS               (UART_RX | UART_TX)

/***************************************************************************//**
*
* uart_init - uart��ʼ��
*
* @param void
*
* @return void
*
*******************************************************************************/
extern void uart_init(void);

/***************************************************************************//**
*
* uart_send - ����ָ�����ȵ�����
*
* @param [in] c Ҫ���͵��ַ�
*
* @return void
*
*******************************************************************************/
extern void uart_send(char c);

/***************************************************************************//**
*
* uart_receive - ����ָ�����ȵ�����
*
* @param [in] pData ���ջ���
* @param [in] size	���ճ���
*
* @return void
*
*******************************************************************************/
extern void uart_receive(uint8 *pData, uint32 size);

/***************************************************************************//**
*
* uart_flush - �ȴ�UART���ͽ���
*
* @param void
*
* @return void
*
*******************************************************************************/
extern void uart_flush(void);

/***************************************************************************//**
*
* uart_receiveNoBlocking - ����0~1���ַ�
*
* @param [in] pData ���ջ���
*
* @return void
*
*******************************************************************************/
extern boolean uart_receiveNoBlocking(uint8 *pData);

/***************************************************************************//**
*
* uart_printf - ���������һ���ַ�
*
* @param [in] c Ҫ���͵��ַ�
*
* @return void
*
*******************************************************************************/
extern void uart_printf(char *pData);

#endif /*__UART_H__ */

/*--------------------------------End of uart.h-------------------------------*/
