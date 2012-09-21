/**
 * @file   : lib.c
 * Fuction : CM3 bootloader lib.
 * By      : LiuNing
 * Date    : 2012-3-10
 */

#include "./inc/lib.h"

/***************************************************************************//**
*
* Function: delay 延时函数
*
* @param [in] time 延时时间
*
* @return void
*
******************************************************************************/
void delay(uint32 time)
{
    while (time--)
    {
    }
}

/***************************************************************************//**
*
* Function: hex2str
*
* Description:
*    将16进制数据转换成字符串
*
* @param [in] *hex 16进制数据
* @param [in] size 16进制数据长度
* @param [in] *str 字符串
*
* @return void
*
*******************************************************************************/
void hex2str(const uint8 *hex, uint32 size, uint8 *str)
{
    uint32 i;
    for (i = 0; i < size; i++)
    {
        str[i * 2] = (hex[i] >> 4) + '0';
        str[i * 2 + 1] = (hex[i] & 0xF) + '0';
    }
    for (i = 0; i < size * 2; i++)
    {
        if (str[i] > '9')
        {
            str[i] += 7;
        }
    }
    str[i] = 0;
}

/***************************************************************************//**
*
* Function: int2str
*
* Description:
*    将数值转换成bcd字符串
*
* @param [in] num  数值
* @param [in] *str 字符串
*
* @return Value 字符串长度
*
*******************************************************************************/
uint8 int2str(const uint32 num, uint8 *str)
{
    int len, i;
    uint32 n;

    n = num;
    for (i = 0; i < 10; i++)
    {
        n = n / 10;
        if (n == 0)
        {
            i++;
            break;
        }
    }
    len = i;
    n = num;
    i--;
    do
    {
        str[i] = n % 10 + '0';
        n = n / 10;
        i--;
    }
    while (i >= 0);
    str[len] = '\0';

    return (len);
}

