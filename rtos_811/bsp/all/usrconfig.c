
#include "bsp.h"

extern void sys_start(void);

/*
 * ��ں���
 * 1. CPU��س�ʼ��
 * 2. ϵͳ�δ��ʼ��
 * 3.
 * 4. ����root����
 */
int main(void)
{
    //BSP_IntDisAll();

    sys_start();    /* never return */

    return 0;
}
