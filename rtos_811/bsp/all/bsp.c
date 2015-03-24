#include <string.h>
#include <bsp.h>
#include <types.h>
#include <maths.h>
#include <LM3Sxxx.h>
#include "lm811uart.h"
#include <intlib.h>

#include <sys_gpio.h>

/*UART口的定义*/
typedef struct
{
    uint32_t  base;
    uint32_t  ttyno;
    uint32_t  intid;

} uart_param_t;
// 串口Channel
static LM811_CHAN lm811UartChan[NUM_TTY];

// UART口的参数表
static const uart_param_t uartParas[] =
{
    {UART0_BASE, 0, INT_UART0},
    {UART1_BASE, 1, INT_UART1},
};

void sysSerialHwInit(void)
{
    uint32_t i;

    for (i = 0; i < ARRAY_SIZE(lm811UartChan); i++)
    {
        lm811UartChan[i].baseregs = uartParas[i].base;
        lm811UartChan[i].ttyno = uartParas[i].ttyno;
        lm811UartDevInit(&lm811UartChan[i]);
    }
}
void sysSerialHwInit2(void)
{
    uint32_t i;
    // 串口
    for (i = 0; i < ARRAY_SIZE(lm811UartChan); i++){
        intConnect(uartParas[i].intid, lm811UartInt, (uint32_t)&lm811UartChan[i]);
        intEnable(uartParas[i].intid);
    }
}
/* serial channel */
SIO_CHAN* sysSerialChanGet(int32_t channel)
{
    int32_t size = ARRAY_SIZE(lm811UartChan);

    if (channel < size)
    {
        return (SIO_CHAN *)&lm811UartChan[channel];
    }
    return NULL;
}

/*IO_MAP的定义*/
typedef struct
{
    uint8_t  iono;
    uint8_t  mode;
    uint32_t gpiobase;
    uint32_t pinno;
} io_map_t;

/*输入IO的对应关系表*/
static const io_map_t the_inout_iomap[] =
{
    {IO_LED0, IO_OUTPUT1, GPIO_PORTD_BASE, GPIO_PIN_0},
    {IO_LED1, IO_OUTPUT1, GPIO_PORTD_BASE, GPIO_PIN_1},
    {IO_LED2, IO_OUTPUT1, GPIO_PORTB_BASE, GPIO_PIN_0},
    {IO_LED3, IO_OUTPUT1, GPIO_PORTB_BASE, GPIO_PIN_1},
    {IO_LED4, IO_OUTPUT1, GPIO_PORTE_BASE, GPIO_PIN_0},
    {IO_LED5, IO_OUTPUT1, GPIO_PORTE_BASE, GPIO_PIN_1},
    {IO_LED6, IO_OUTPUT1, GPIO_PORTC_BASE, GPIO_PIN_5},
    //{IO_BUTTON ,GPIO_PORTD_BASE  ,GPIO_PIN_0},
};
static status_t
bsp_gpio_cfg(int32_t io_no, int32_t value)
{
    uint32_t i = 0;

    for(i = 0; i < ARRAY_SIZE(the_inout_iomap); i++)
    {
        if(io_no == the_inout_iomap[i].iono)
        {
            switch (value)
            {
                case IO_INPUT:
                    GPIODirModeSet(the_inout_iomap[i].gpiobase, the_inout_iomap[i].pinno, GPIO_DIR_MODE_IN);
                    break;
                case IO_OUTPUT0:
                    GPIODirModeSet(the_inout_iomap[i].gpiobase, the_inout_iomap[i].pinno, GPIO_DIR_MODE_OUT);
                    GPIOPinWrite(the_inout_iomap[i].gpiobase, the_inout_iomap[i].pinno, ~the_inout_iomap[i].pinno);
                    break;
                case IO_OUTPUT1:
                    GPIODirModeSet(the_inout_iomap[i].gpiobase, the_inout_iomap[i].pinno, GPIO_DIR_MODE_OUT);
                    GPIOPinWrite(the_inout_iomap[i].gpiobase, the_inout_iomap[i].pinno, the_inout_iomap[i].pinno);
                    break;
                default:
                    return ERROR;
            }
        }
    }

    return ERROR;
}

static status_t
bsp_gpio_write(int32_t io_no, int32_t value)
{
    uint32_t i = 0;

    for(i = 0; i < ARRAY_SIZE(the_inout_iomap); i++)
    {
        if(io_no == the_inout_iomap[i].iono)
        {
            if(value == 1)
            {
                GPIOPinWrite(the_inout_iomap[i].gpiobase, the_inout_iomap[i].pinno, the_inout_iomap[i].pinno);
            }
            else
            {
                GPIOPinWrite(the_inout_iomap[i].gpiobase, the_inout_iomap[i].pinno, ~the_inout_iomap[i].pinno);
            }
            return  OK;
        }
    }

    return ERROR;
}
static status_t
bsp_gpio_read (int32_t io_no)
{
    uint32_t i = 0;

    for(i = 0; i < sizeof(the_inout_iomap)/sizeof(io_map_t); i++)
    {
        if(io_no == the_inout_iomap[i].iono)
        {
            return (status_t)GPIOPinRead(the_inout_iomap[i].gpiobase, the_inout_iomap[i].pinno);
        }
    }

    return ERROR;
}
static void
bsp_gpio_init(void)
{
    int32_t i;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    for (i = 0; i < ARRAY_SIZE(the_inout_iomap); i++)
    {
        if (the_inout_iomap[i].mode == IO_INPUT)
        {
            GPIODirModeSet(the_inout_iomap[i].gpiobase, the_inout_iomap[i].pinno, GPIO_DIR_MODE_IN);
        }
        else
        {
            GPIODirModeSet(the_inout_iomap[i].gpiobase, the_inout_iomap[i].pinno, GPIO_DIR_MODE_OUT);
            GPIOPadConfigSet(the_inout_iomap[i].gpiobase, the_inout_iomap[i].pinno, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
        }
    }

    sys_gpio_bspInstall(bsp_gpio_cfg, bsp_gpio_read, bsp_gpio_write);

    /* 设置uart0、1模式 */
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

}

void bsp_reboot(void)
{
    SysCtlReset();
}

void sysHwInit0(void)
{
    intLock();

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

    intLibInit();

    //系统IO初始化
    bsp_gpio_init();

    //系统串口初始化
    sysSerialHwInit();

    intUnlock();

}
void  sysHwInit ()
{
    sysSerialHwInit2();
}


SEM_ID the_lock = NULL;
uint32_t ADC_EndFlag = 0;

// ADC采样序列0的中断函数
void ADC_Sequence_0_ISR(void)
{
    unsigned long ulStatus;
    ulStatus = ADCIntStatus(ADC_BASE, 0, true); // 读取中断状态
    ADCIntClear(ADC_BASE, 0); // 清除中断状态，重要,等待下次AD中断
    semGive(the_lock);
//    if (ulStatus != 0) // 如果中断状态有效
//    {
//        ADC_EndFlag = 1; // 置位ADC采样结束标志
//    }
}
//   ADC初始化
void
adc_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC); //使能ADC模块
    SysCtlADCSpeedSet(SYSCTL_ADCSPEED_500KSPS); //设置ADC采样速率
    ADCSequenceDisable(ADC_BASE, 0); // 配置前先禁止采样序列

    ADCSequenceConfigure(ADC_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    //配置ADC采样序列的触发事件和优先级：ADC基址，采样序列编号，触发事件，采样优先级
    ADCSequenceStepConfigure(ADC_BASE, 0, 0,
            ADC_CTL_END | ADC_CTL_CH0 | ADC_CTL_IE);

    //配置ADC采样序列发生器的步进 ：ADC基址，采样序列编号，步值，通道设置

    intConnect(INT_ADC0, ADC_Sequence_0_ISR, 0u);
    ADCIntEnable(ADC_BASE, 0); //使能ADC采样序列的中断
//    IntEnable(INT_ADC0); // 使能ADC采样序列中断
    intEnable(INT_ADC0);
    IntMasterEnable(); // 使能处理器中断

    ADCSequenceEnable(ADC_BASE, 0); // 使能一个ADC采样序列
    the_lock = semBCreate(0);
//    printf("%x\n", the_lock);

}

uint32_t
adc_get(void)
{
    unsigned long ulValue;
    ADCProcessorTrigger(ADC_BASE, 0);
    // 处理器触发采样序，调用ADCProcessorTrigger( )函数触发ADC采样
//    while (!ADC_EndFlag)  // 等待采样结束
//    {
////        ;
//        taskDelay(1);
//    }
    if (OK != semTake(the_lock, 1000))
    {
        return 0;
    }
    ADC_EndFlag = 0; // 清除ADC采样结束标志
    ADCSequenceDataGet(ADC_BASE, 0, &ulValue); // 读取ADC转换结果
    return (ulValue);
}

void sysHwInit2()
{
    //gpio_init(true);

#ifdef INCLUDE_DATAFLASH
    ftlInit();
#endif

}
#if 0
int32_t bsp_getchar(void)
{
    int32_t c;

    c = (int32_t)UARTCharGetNonBlocking(UART0_BASE);
    if (c < 0)
        c = 0;
    return c;
}

void bsp_putchar(char_t c)
{
    UARTCharPut(UART0_BASE, c);
}
#endif
void bsp_feeddog(void)
{

}
