#include <sched.h>
#include <leds.h>
#include <string.h>

/** 记录每个心跳毫秒数 */

uint16_t total = 0;
uint16_t pos = 0;
uint16_t the_heart_t[60];

uint32_t pre_tick; //上一心跳时间片
uint32_t heart_tick; //上一心跳时间片

void
heart_show(void)
{
    int i;
    int total_v = 0;
    int count = total;

    for (i = 0; i < count; i++)
    {
        total_v += the_heart_t[i];
    }
    total_v = (60 * 10000) * total_v / count;
    printf("心率: %d.%dHz", total_v / 10, total_v % 10);
}

uint32_t
get_v(void)
{
    uint32_t max;
    uint32_t min;
    uint32_t total;
    uint32_t tmp;

    tmp = adc_get();
    total = max = min = tmp;

    for(int i = 1; i < 4; i++)
    {
        tmp = adc_get();
        total += tmp;

        if (tmp > max)
        {
            max = tmp;
        }
        if (tmp < min)
        {
            min = tmp;
        }
    }

    return (total - max - min) >> 1;
}



void
usrAppInit(void)
{
    printf("In usr usrAppInit\n");
//    task_leds_start();
    int i = 0;
    uint32_t cnt = 0;
    uint32_t heart_5[5];
    uint32_t heart;
    uint32_t max;
    uint32_t min;
    uint32_t total;
    adc_init();

    OSTaskChangePrio(0, 7);

    uint32_t ad;
    uint32_t pre_ad = get_v();
    pre_tick = tickGet();
    while (1)
    {
        ad = get_v();
        if ((pre_ad < 600) && (ad >= 600))
        {
//            printf("%d pre_ad:%d ad:%d", i++, pre_ad, ad);
            heart_tick = tickGet();
            pre_tick = heart_tick - pre_tick;
            heart = (1200000 / pre_tick);
            if (heart > 35*10 && heart < 180*10)
            {
                memmove(&heart_5[0], &heart_5[1], 4*sizeof(uint32_t));
                heart_5[4] = heart;
                if (cnt < 5)
                {
                    cnt++;
                }
                else
                {
                    total = max = min = heart_5[0];
                    for(int i = 1; i < 5; i++)
                    {
                        total += heart_5[i];

                        if (heart_5[i] > max)
                        {
                            max = heart_5[i];
                        }
                        if (heart_5[i] < min)
                        {
                            min = heart_5[i];
                        }
                    }
                    heart = (total - max - min) / 3;

                    printf("心率:%d.%d\n", heart / 10,  heart % 10);//pre_tick
                }
            }

            pre_tick = heart_tick;
        }
        pre_ad = ad;
        taskDelay(2);
    }
}
