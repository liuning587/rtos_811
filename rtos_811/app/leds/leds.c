#include <types.h>
#include <sched.h>
#include <sys_gpio.h>
#include <leds.h>

#define OS_TASK_LEDS_PRIO   7u
#define TASK_LEDS_STK_SIZE  100
uint32_t the_leds_stack[TASK_LEDS_STK_SIZE / 4];

static void leds_loop(void);

void task_leds_start(void)
{
    taskSpawn("LEDS",
            OS_TASK_LEDS_PRIO,
            the_leds_stack,
            TASK_LEDS_STK_SIZE,
            (OSFUNCPTR)leds_loop, 0);
}


static void leds_loop(void)
{
    gpio_out_e leds = IO_LED0;
    while (1)
    {
        sys_gpio_write(leds, E_LED_ON);
        taskDelay(25);
        sys_gpio_write(leds, E_LED_OFF);
        taskDelay(20);

        leds++;
        if (leds > IO_LED6)
        {
            leds = IO_LED0;
        }
    }
}
