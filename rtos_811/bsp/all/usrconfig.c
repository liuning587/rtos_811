
#include "bsp.h"
#include <ttylib.h>
#include <string.h>


extern void sysHwInit0();
extern void sysHwInit ();
extern void sysHwInit2();
extern void    OSInit();
extern void    OSStart();
extern void shellInit();
extern SIO_CHAN * sysSerialChanGet();
extern void usrAppInit();
extern void OS_CPU_SysTickInit();
extern status_t excInit();
extern void  OSStatInit (void);

uint32_t consoleFd;      /* fd of initial console device */
uint8_t consoleName[5];    /* console device name, eg. "ttyc0" */


static uint32_t rootstack[ROOTSTACKSIZE / 4];

void rootTask()
{
    //��ʼ��ϵͳ����
    OS_CPU_SysTickInit( (SysCtlClockGet() / OS_TICKS_PER_SEC));

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();
#endif

    if (NUM_TTY > 0)
    {
        uint32_t ix;
        uint8_t tyName[5];
        for (ix = 0; ix < NUM_TTY; ix++)    /* create serial devices */
        {
              memcpy(tyName,"ttyc",4);
              tyName[4] = (uint8_t)ix+'0';
              (void) ttyDevCreate (tyName, sysSerialChanGet(ix));

            if (ix == CONSOLE_TTY)      /* init the tty console */
            {
                memcpy (consoleName, tyName,5);
                consoleFd = ttyOpen(consoleName);
                ttyIoctl(consoleFd,SIO_BAUD_SET, 115200);
                ttyIoctl(consoleFd,SIO_HW_OPTS_SET,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                UART_CONFIG_PAR_NONE));
            }
        }
    }

    sysHwInit2();
    shellInit();
    excInit();
    //�û�����
    usrAppInit();
}
int main (void)
{
    //���ж� ��ϵͳ��Ƶ
    //�ж��������ʼ��
    //IO��ʼ��TTY��ʼ��
    sysHwInit0();
    //
    //ϵͳ�ں�����
    //
    OSInit();

    //��TTY �� �ж�
    //���������
    sysHwInit ();

    //���������ʱ�ӽ��ĳ�ʼ��
    taskSpawn("root",0,rootstack, ROOTSTACKSIZE,(OSFUNCPTR)rootTask,0);

    OSStart();
}

