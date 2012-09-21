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

/** 计时类型 */
typedef enum e_timer_type
{
    TT_WAIT_KEY = 0,       /* 等待按键计时 */
    TT_WAIT_DATA,          /* 等待数据     */
} E_TIMER_TYPE;

static volatile uint32 g_currTime = 0;  // 当前时间(累计秒数)
static volatile E_TIMER_TYPE g_timerType = TT_WAIT_KEY;       //  当前计时类型

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
    // 初始化设备
    if (OK != hw_init()) return;

    uart_printf("\r\n");
    uart_printf("*********************************************\r\n");
    uart_printf("*           GNU BootLoader v1.5             *\n\r");
    uart_printf("*        Copyright 2012 By LiuNing.         *\r\n");
    uart_printf("*********************************************\r\n");
    // 尝试通过串口升级
    if (OK != uart_update()) return;

    g_timerType = TT_WAIT_DATA;

    // 关闭定时器
    HWREG(NVIC_ST_CTRL) &= ~(NVIC_ST_CTRL_INTEN);

    // 启动应用系统
    uart_printf("Enter application ..\r\n");

    return;
}

/***************************************************************************//**
*
* hw_init - 硬件初始化
*
* @param void
*
* @retval true  成功
* @retval false 失败
*
*******************************************************************************/
LOCAL STATUS hw_init(void)
{
    // 配置时钟
    // 使能主振荡器
    HWREG(SYSCTL_RCC) &= ~(SYSCTL_RCC_MOSCDIS);
    // 等待主振荡器稳定
    delay(524288);
    // 选择主振荡器作为振荡源
    HWREG(SYSCTL_RCC) = ((HWREG(SYSCTL_RCC) & ~(SYSCTL_RCC_OSCSRC_M))
                         | SYSCTL_RCC_OSCSRC_MAIN);

    // 配置UART
    uart_init();

    // 配置片内flash
    HWREG(FLASH_USECRL) = ((CRYSTAL_FREQ + 999999) / 1000000) - 1;

    // 开启计数器，每秒产生一个中断
    HWREG(NVIC_ST_RELOAD) = CRYSTAL_FREQ;
    HWREG(NVIC_ST_CTRL) = (NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_INTEN
                           | NVIC_ST_CTRL_ENABLE);

    return (OK);
}

/***************************************************************************//**
*
* sysTickIntHandler - 定时器中断
*
* @param void
*
* @return void
*
*******************************************************************************/
void sysTickIntHandler ( void )
{
    g_currTime++;

    // 等待按键时，每秒打印一个"."
    if (g_timerType == TT_WAIT_KEY) uart_send('.');
}

/***************************************************************************//**
*
* escKeyDetect - 按键检测
*
* @param void
*
* @retval 返回超级终端的按键值
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

