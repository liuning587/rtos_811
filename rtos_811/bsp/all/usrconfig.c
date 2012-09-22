
#include "bsp.h"

extern void sys_start(void);

/*
 * 入口函数
 * 1. CPU相关初始化
 * 2. 系统滴答初始化
 * 3.
 * 4. 创建root任务
 */
int main(void)
{
    //BSP_IntDisAll();

    sys_start();    /* never return */

    return 0;
}
