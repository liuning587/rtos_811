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
* Function: delay ��ʱ����
*
* @param [in] time ��ʱʱ��
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
*	 ��16��������ת�����ַ���
*
* @param [in] *hex 16��������
* @param [in] size 16�������ݳ���
* @param [in] *str �ַ���
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
*	 ����ֵת����bcd�ַ���
*
* @param [in] num  ��ֵ
* @param [in] *str �ַ���
*
* @return Value �ַ�������
*
*******************************************************************************/
extern uint8 int2str(const uint32 num, uint8 *str);

#endif /* _LIB_H_ */

