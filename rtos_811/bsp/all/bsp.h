#ifndef __BSP_H__
#define __BSP_H__

#include <ucos_ii.h>
#include <LM3Sxxx.h>

/**
 *  System Clock(CCLK) Setting   系统时钟(CCLK) 设定
 *  CCLK must be less than or equal to  20MHz/50MHz(depend on the max. cclk of the CPU)
 *  CCLK必须小于或等于20MHz/50MHz(根据单片机的最大CCLK而定)
 *  If PLL_EN=0, CCLK=EXT_CLK/CCLK_DIV, CCLK must <=20MHZ/50MHz
 *  If PLL_EN>0, CCLK=200MHz/CCLK_DIV, CCLK must <=20MHZ/50MHz
 */
#define  EXT_CLK            SYSCTL_XTAL_6MHZ    /* external clock 外部时钟      */
#define  PLL_EN             0                   /* 1：Enable PLL  使能PLL   */
#define  CCLK_DIV           SYSCTL_SYSDIV_1     /* CCLK divider CCLK分频系数 */

extern void BSP_Init(void);

#endif /* __BSP_H__ */
