/**
 * @file   : main.c
 * Fuction : CM3 bootloader main fuction
 * By      : LiuNing
 * Date    : 2012-3-10
 */
#include "./inc/lm3s.h"
#include "./inc/cfg.h"
#include "./inc/iflash.h"
#include "./inc/packet.h"
#include "./inc/uart.h"
#include "./inc/lib.h"

/** ��ʱ���� */
typedef enum e_timer_type
{
    TT_WAIT_KEY = 0,       /* �ȴ�������ʱ */
    TT_WAIT_DATA,          /* �ȴ�����     */
} E_TIMER_TYPE;

static volatile uint32 g_currTime = 0;  // ��ǰʱ��(�ۼ�����)
static volatile E_TIMER_TYPE g_timerType = TT_WAIT_KEY;       //  ��ǰ��ʱ����

static STATUS hw_init(void);
extern STATUS uart_update(void);

/***************************************************************************//**
*
* updater
*
* @param void
*
* @return void
*
*******************************************************************************/
void updater(void)
{
    // ��ʼ���豸
    if (OK != hw_init()) return;

    uart_printf("\r\n");
    uart_printf("*********************************************\r\n");
    uart_printf("*           GNU BootLoader v1.5             *\n\r");
    uart_printf("*        Copyright 2012 By LiuNing.         *\r\n");
    uart_printf("*********************************************\r\n");
    // ����ͨ����������
    if (OK != uart_update()) return;

    g_timerType = TT_WAIT_DATA;

    // �رն�ʱ��
    HWREG(NVIC_ST_CTRL) &= ~(NVIC_ST_CTRL_INTEN);

    // ����Ӧ��ϵͳ
    uart_printf("Enter application ..\r\n");

    return;
}

/***************************************************************************//**
*
* hw_init - Ӳ����ʼ��
*
* @param void
*
* @retval true  �ɹ�
* @retval false ʧ��
*
*******************************************************************************/
LOCAL STATUS hw_init(void)
{
    // ����ʱ��
    // ʹ��������
    HWREG(SYSCTL_RCC) &= ~(SYSCTL_RCC_MOSCDIS);
    // �ȴ��������ȶ�
    delay(524288);
    // ѡ����������Ϊ��Դ
    HWREG(SYSCTL_RCC) = ((HWREG(SYSCTL_RCC) & ~(SYSCTL_RCC_OSCSRC_M))
                         | SYSCTL_RCC_OSCSRC_MAIN);

    // ����UART
    uart_init();

    // ����Ƭ��flash
    HWREG(FLASH_USECRL) = ((CRYSTAL_FREQ + 999999) / 1000000) - 1;

    // ������������ÿ�����һ���ж�
    HWREG(NVIC_ST_RELOAD) = CRYSTAL_FREQ;
    HWREG(NVIC_ST_CTRL) = (NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_INTEN
                           | NVIC_ST_CTRL_ENABLE);

    return (OK);
}

/***************************************************************************//**
*
* sysTickIntHandler - ��ʱ���ж�
*
* @param void
*
* @return void
*
*******************************************************************************/
void sysTickIntHandler ( void )
{
    g_currTime++;

    // �ȴ�����ʱ��ÿ���ӡһ��"."
    if (g_timerType == TT_WAIT_KEY) uart_send('.');
}

/***************************************************************************//**
*
* escKeyDetect - �������
*
* @param void
*
* @retval ���س����ն˵İ���ֵ
*
*******************************************************************************/
extern boolean escKeyDetect(void)
{
    uint8 key;
    boolean result = false;

    while(true)
    {
        if (MAX_WAITKEY_TIME <= g_currTime)
            break;
        key = 0;
        if (uart_receiveNoBlocking(&key) && (27 == key))
        {
            g_timerType = TT_WAIT_DATA;
            result = true;
            break;
        }
    }
    uart_printf("\r\n");

    return (result);
}

