/**
 * @file   : iflash.c
 * Fuction : �ڲ�flash����.
 * By      : LiuNing
 * Date    : 2012-3-10
 */

#include "./inc/lm3s.h"
#include "./inc/iflash.h"

/***************************************************************************//**
*
* iflash_erase - ����һ��
*
* @param [in] address ������ַ
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
* iflash_read - ��ȡ
*
* @param [in] pData   ��ȡ����
* @param [in] address ��ȡ��ַ
* @param [in] length  ��ȡ����
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
* iflash_write - д��
*
* @param [in] destAddr д���ַ
* @param [in] pSrcData ��д���ַ
* @param [in] length   д�볤��
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
* iflash_sizeGet - ��ȡflash��С
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
* iflash_spaceCheck - д���ַ����С�Ϸ��Լ��
*
* @param [in] address д���ַ
* @param [in] imgSize д�볤��
*
* @retval true  �Ϸ�
* @retval false �Ƿ�
*
*******************************************************************************/
boolean iflash_spaceCheck(uint32 address, uint32 imgSize)
{
    uint32 flashSize;

    /** ��ȡflash����*/
    flashSize = ((HWREG(SYSCTL_DC0) & SYSCTL_DC0_FLASHSZ_M) + 1) << 11;

    if (((address != 0) && (address != APP_START_ADDRESS)) /* ��������bootloaderʱ����ʼ��ַ�����ò�ͬ*/
            || ((address + imgSize) > flashSize) /* ������ַ����flash����*/
            || ((imgSize & 3) != 0)              /* д���Сδ���ֶ���*/
       )
        return (false);
    else
        return (true);
}

/***************************************************************************//**
*
* iflash_errorClear - ��������ж�״̬
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
* iflash_errorCheck - ����ж�״̬
*
* @param void
*
* @retval true  ״̬����
* @retval false δ����
*
*******************************************************************************/
boolean iflash_errorCheck(void)
{
    return ((HWREG(FLASH_FCRIS) & FLASH_FCRIS_ARIS) ? (true) : (false));
}

/*-------------------------------End of flash.c-------------------------------*/
