/**
 * @file   : iflash.h
 * Fuction : 内部flash.
 * By      : LiuNing
 * Date    : 2012-3-10
 */

#ifndef _IFLASH_H_
#define _IFLASH_H_

#include "cfg.h"

/***************************************************************************//**
*
* iflash_erase - 擦除一块
*
* @param [in] address 擦除地址
*
* @return void
*
*******************************************************************************/
extern void iflash_erase(uint32 address);

/***************************************************************************//**
*
* iflash_write - 写入
*
* @param [in] destAddr 写入地址
* @param [in] pSrcData 待写入地址
* @param [in] length   写入长度
*
* @return void
*
*******************************************************************************/
extern void iflash_write(uint32 destAddr, uint8 *pSrcData, uint32 length);

/***************************************************************************//**
*
* iflash_read - 读取
*
* @param [in] pData   读取缓冲
* @param [in] address 读取地址
* @param [in] length  读取长度
*
* @return void
*
*******************************************************************************/
extern void iflash_read(uint8 *pData, uint32 address, uint32 length);

/***************************************************************************//**
*
* iflash_sizeGet - 获取flash大小
*
* @param void
*
* @return void
*
*******************************************************************************/
extern uint32 iflash_sizeGet(void);

/***************************************************************************//**
*
* iflash_spaceCheck - 写入地址及大小合法性检查
*
* @param [in] address 写入地址
* @param [in] imgSize 写入长度
*
* @retval true	合法
* @retval false 非法
*
*******************************************************************************/
extern boolean iflash_spaceCheck(uint32 address, uint32 imgSize);

/***************************************************************************//**
*
* iflash_errorClear - 清除错误中断状态
*
* @param void
*
* @return void
*
*******************************************************************************/
extern boolean iflash_errorCheck(void);

/***************************************************************************//**
*
* iflash_errorCheck - 检查中断状态
*
* @param void
*
* @retval true	状态错误
* @retval false 未出错
*
*******************************************************************************/
extern void iflash_errorClear(void);


#endif /* _IFLASH_H_ */

