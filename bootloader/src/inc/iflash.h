/**
 * @file   : iflash.h
 * Fuction : �ڲ�flash.
 * By      : LiuNing
 * Date    : 2012-3-10
 */

#ifndef _IFLASH_H_
#define _IFLASH_H_

#include "cfg.h"

/***************************************************************************//**
*
* iflash_erase - ����һ��
*
* @param [in] address ������ַ
*
* @return void
*
*******************************************************************************/
extern void iflash_erase(uint32 address);

/***************************************************************************//**
*
* iflash_write - д��
*
* @param [in] destAddr д���ַ
* @param [in] pSrcData ��д���ַ
* @param [in] length   д�볤��
*
* @return void
*
*******************************************************************************/
extern void iflash_write(uint32 destAddr, uint8 *pSrcData, uint32 length);

/***************************************************************************//**
*
* iflash_read - ��ȡ
*
* @param [in] pData   ��ȡ����
* @param [in] address ��ȡ��ַ
* @param [in] length  ��ȡ����
*
* @return void
*
*******************************************************************************/
extern void iflash_read(uint8 *pData, uint32 address, uint32 length);

/***************************************************************************//**
*
* iflash_sizeGet - ��ȡflash��С
*
* @param void
*
* @return void
*
*******************************************************************************/
extern uint32 iflash_sizeGet(void);

/***************************************************************************//**
*
* iflash_spaceCheck - д���ַ����С�Ϸ��Լ��
*
* @param [in] address д���ַ
* @param [in] imgSize д�볤��
*
* @retval true	�Ϸ�
* @retval false �Ƿ�
*
*******************************************************************************/
extern boolean iflash_spaceCheck(uint32 address, uint32 imgSize);

/***************************************************************************//**
*
* iflash_errorClear - ��������ж�״̬
*
* @param void
*
* @return void
*
*******************************************************************************/
extern boolean iflash_errorCheck(void);

/***************************************************************************//**
*
* iflash_errorCheck - ����ж�״̬
*
* @param void
*
* @retval true	״̬����
* @retval false δ����
*
*******************************************************************************/
extern void iflash_errorClear(void);


#endif /* _IFLASH_H_ */

