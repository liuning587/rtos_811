/**
 * @file   : iflash.c
 * Fuction : 内部flash操作.
 * By      : LiuNing
 * Date    : 2012-3-10
 */

#include "./inc/lm3s.h"
#include "./inc/iflash.h"

/***************************************************************************//**
*
* iflash_erase - 擦除一块
*
* @param [in] address 擦除地址
*
* @return void
*
*******************************************************************************/
void iflash_erase(uint32 address)
{
    HWREG(FLASH_FMA) = address;
    HWREG(FLASH_FMC) = FLASH_FMC_WRKEY | FLASH_FMC_ERASE;

    while(HWREG(FLASH_FMC) & FLASH_FMC_ERASE)
    {
    }
}

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
void iflash_read(uint8 *pData, uint32 address, uint32 length)
{
    uint32 i;

    for (i = 0; i < length; i ++)
    {
        *pData++ = (*((uint8 *) address++));
    }
}

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
void iflash_write(uint32 destAddr, uint8 *pSrcData, uint32 length)
{
    uint32 i;

    for (i = 0; i < length; i += 4)
    {
        HWREG(FLASH_FMA) = destAddr + i;
        HWREG(FLASH_FMD) = *(unsigned long *)(pSrcData + i);
        HWREG(FLASH_FMC) = FLASH_FMC_WRKEY | FLASH_FMC_WRITE;

        while (HWREG(FLASH_FMC) & FLASH_FMC_WRITE)
        {
        }
    }
}

/***************************************************************************//**
*
* iflash_sizeGet - 获取flash大小
*
* @param void
*
* @return void
*
*******************************************************************************/
uint32 iflash_sizeGet(void)
{
    return (((HWREG(SYSCTL_DC0) & SYSCTL_DC0_FLASHSZ_M) + 1) << 11);
}

/***************************************************************************//**
*
* iflash_spaceCheck - 写入地址及大小合法性检查
*
* @param [in] address 写入地址
* @param [in] imgSize 写入长度
*
* @retval true  合法
* @retval false 非法
*
*******************************************************************************/
boolean iflash_spaceCheck(uint32 address, uint32 imgSize)
{
    uint32 flashSize;

    /** 获取flash容量*/
    flashSize = ((HWREG(SYSCTL_DC0) & SYSCTL_DC0_FLASHSZ_M) + 1) << 11;

    if (((address != 0) && (address != APP_START_ADDRESS)) /* 不是升级bootloader时，起始地址与配置不同*/
            || ((address + imgSize) > flashSize) /* 结束地址超出flash容量*/
            || ((imgSize & 3) != 0)              /* 写入大小未按字对齐*/
       )
        return (false);
    else
        return (true);
}

/***************************************************************************//**
*
* iflash_errorClear - 清除错误中断状态
*
* @param void
*
* @return void
*
*******************************************************************************/
void iflash_errorClear(void)
{
    HWREG(FLASH_FCMISC) = FLASH_FCMISC_AMISC;
}

/***************************************************************************//**
*
* iflash_errorCheck - 检查中断状态
*
* @param void
*
* @retval true  状态错误
* @retval false 未出错
*
*******************************************************************************/
boolean iflash_errorCheck(void)
{
    return ((HWREG(FLASH_FCRIS) & FLASH_FCRIS_ARIS) ? (true) : (false));
}

/*-------------------------------End of flash.c-------------------------------*/
