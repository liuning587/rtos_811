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
* uart_init - uart初始化
*
* @param void
*
* @return void
*
*******************************************************************************/
extern void uart_init(void);

/***************************************************************************//**
*
* uart_send - 发送指定长度的数据
*
* @param [in] c 要发送的字符
*
* @return void
*
*******************************************************************************/
extern void uart_send(char c);

/***************************************************************************//**
*
* uart_receive - 接收指定长度的数据
*
* @param [in] pData 接收缓冲
* @param [in] size	接收长度
*
* @return void
*
*******************************************************************************/
extern void uart_receive(uint8 *pData, uint32 size);

/***************************************************************************//**
*
* uart_flush - 等待UART发送结束
*
* @param void
*
* @return void
*
*******************************************************************************/
extern void uart_flush(void);

/***************************************************************************//**
*
* uart_receiveNoBlocking - 接收0~1个字符
*
* @param [in] pData 接收缓冲
*
* @return void
*
*******************************************************************************/
extern boolean uart_receiveNoBlocking(uint8 *pData);

/***************************************************************************//**
*
* uart_printf - 往串口输出一个字符
*
* @param [in] c 要发送的字符
*
* @return void
*
*******************************************************************************/
extern void uart_printf(char *pData);

#endif /*__UART_H__ */

/*--------------------------------End of uart.h-------------------------------*/
