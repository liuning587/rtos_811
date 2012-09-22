#include <bsp.h>
#include <types.h>
#include <LM3Sxxx.h>

static  void  tickInit (void)
{
    SysTickPeriodSet((INT32U)(SysCtlClockGet() / OS_TICKS_PER_SEC) -1 );
    SysTickEnable();
    SysTickIntEnable();
}

void BSP_Init(void)
{
#if PLL_EN == 0
    /*  Not use PLL  不使用PLL      */
    SysCtlClockSet(CCLK_DIV
            | SYSCTL_USE_OSC
            | SYSCTL_OSC_MAIN
            | EXT_CLK);
 #else
    /*  Use PLL  使用PLL            */
    SysCtlClockSet(CCLK_DIV
            | SYSCTL_USE_PLL
            | SYSCTL_OSC_MAIN
            | EXT_CLK);
#endif

    /*  Initialize the uC/OS-II tick*/
    tickInit();
}

int32_t bsp_getchar(void)
{
    return 0;
}

void bsp_putchar(char_t c)
{

}
