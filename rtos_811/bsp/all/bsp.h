#ifndef __BSP_H__
#define __BSP_H__

#include <ucos_ii.h>
#include <LM3Sxxx.h>
#include <types.h>

/**
 *  System Clock(CCLK) Setting   ϵͳʱ��(CCLK) �趨
 *  CCLK must be less than or equal to  20MHz/50MHz(depend on the max. cclk of the CPU)
 *  CCLK����С�ڻ����20MHz/50MHz(���ݵ�Ƭ�������CCLK����)
 *  If PLL_EN=0, CCLK=EXT_CLK/CCLK_DIV, CCLK must <=20MHZ/50MHz
 *  If PLL_EN>0, CCLK=200MHz/CCLK_DIV, CCLK must <=20MHZ/50MHz
 */
#define  EXT_CLK            SYSCTL_XTAL_6MHZ    /* external clock �ⲿʱ��      */
#define  PLL_EN             1                   /* 1��Enable PLL  ʹ��PLL   */
#define  CCLK_DIV           SYSCTL_SYSDIV_1     /* CCLK divider CCLK��Ƶϵ�� */

#define NUM_TTY             1   /* TTY���� */
#define CONSOLE_TTY         0   /* �ն�TTY */
#define ROOTSTACKSIZE    1000   /* root������ջ */

extern uint32_t consoleFd;      /* fd of initial console device */
extern uint8_t consoleName[5];  /* console device name, eg. "ttyc0" */

#endif /* __BSP_H__ */
