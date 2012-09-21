/**
 * @file   : lib.h
 * Fuction : CM3 bootloader lib.
 * By      : LiuNing
 * Date    : 2012-3-10
 */

#ifndef _LIB_H_
#define _LIB_H_

#include "cfg.h"

/***************************************************************************//**
*
* Function: delay 延时函数
*
* @param [in] time 延时时间
*
* @return void
*
******************************************************************************/
extern void delay(uint32 time);

/***************************************************************************//**
*
* Function: hex2str
*
* Description:
*	 将16进制数据转换成字符串
*
* @param [in] *hex 16进制数据
* @param [in] size 16进制数据长度
* @param [in] *str 字符串
*
* @return void
*
*******************************************************************************/
extern void hex2str(const uint8 *hex, uint32 size, uint8 *str);

/***************************************************************************//**
*
* Function: int2str
*
* Description:
*	 将数值转换成bcd字符串
*
* @param [in] num  数值
* @param [in] *str 字符串
*
* @return Value 字符串长度
*
*******************************************************************************/
extern uint8 int2str(const uint32 num, uint8 *str);

#endif /* _LIB_H_ */

