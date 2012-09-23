#include <stdio.h>
#include <types.h>
#include <sched.h>
#include <ucos_ii.h>

#include <shell.h>
#include <exc.h>

extern void BSP_Init(void);
extern void usrAppInit(void);
static void root_task(void *p_arg);

#define  ROOTSTACKSIZE  (200)      /* ����root task��ջ��С */
static  uint32_t rootstack[ROOTSTACKSIZE / 4];

void sys_start(void)
{
    OSInit();
    /*���������ʱ�ӽ��ĳ�ʼ��*/
    taskSpawn("root", 0, rootstack, ROOTSTACKSIZE,(OSFUNCPTR)root_task, 0);

    OSStart();
}

static void
root_task(void *p_arg)
{
    BSP_Init();
    //printf version
    shellInit();
    usrAppInit();
    excInit();
}
