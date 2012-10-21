/**
 ******************************************************************************
 * @file       ttylib.c
 * @version    V1.1.4
 * @brief      API C source file of ttylib.c
 *             Created on: 2012-9-25
 *             Author: Administrator
 * @details    This file including all API functions's implement of dps.
 * @copy       Copyrigth(C), 2008-2012,liuning
 *
 ******************************************************************************
 */

/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include <ttylib.h>
#include <string.h>
#include <maths.h>


/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/

#undef LONGBUF

/*-----------------------------------------------------------------------------
 Section: static Variable
 ----------------------------------------------------------------------------*/

#ifdef LONGBUF
char lrbuf[MAXLONGBUF];
char lrbufused = 0;
#endif
//char lwbuf[MAXLONGBUF];

#define TY_NUM            2u                /* TTY最大个数 */
#define TY_BUFSIZE      128u                /* 每个TTY的BUF大小 */
static TY_DEV ty_devs[TY_NUM];              /* TTY设备 */
static char_t rbuf[TY_NUM * TY_BUFSIZE];    /* TTY接收缓存 */
static char_t wbuf[TY_NUM * TY_BUFSIZE];    /* TTY发送缓存 */
/*-----------------------------------------------------------------------------
 Section: static Function Prototypes
 ----------------------------------------------------------------------------*/
static status_t tyITx(uint32_t ttyno, uint8_t *pChar);
static status_t tyIRd(uint32_t ttyno, uint8_t inchar);

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/


/**
 ******************************************************************************
 * @brief      tty设备结构初始化
 * @param[in]  None
 * @param[out] None
 * @retval     None
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
void ttyDrv(void)
{
    memset(ty_devs, 0, TY_NUM * sizeof(TY_DEV));
    memset(rbuf, 0, TY_NUM * TY_BUFSIZE);
    memset(wbuf, 0, TY_NUM * TY_BUFSIZE);

    return ;
}


/**
 ******************************************************************************
 * @brief      创建TTY设备
 * @param[in]  uint8_t *name        TTY设备名
 * @param[in]   SIO_CHAN *pChan     pSioChan:  pointer to core driver structure
 * @retval     OK 创建成功
 * @retval  ERROR 创建失败
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
status_t ttyDevCreate(uint8_t *name, SIO_CHAN *pChan)
{
    uint8_t i = 0;
    // 定位尚未创建的dev
    while ((0 != ty_devs[i].fd) && (i < TY_NUM))
        i++;
    if (i >= TY_NUM) return ERROR;

    memcpy(ty_devs[i].name, name, 5);
    ty_devs[i].fd = i + 1;
    ty_devs[i].pSioChan = pChan;
    ty_devs[i].rdSyncSem = semBCreate(1);
    ty_devs[i].wrtSyncSem = semBCreate(1);
    ty_devs[i].rdBuf.bufSize = TY_BUFSIZE;
    ty_devs[i].wrtBuf.bufSize = TY_BUFSIZE;
    ty_devs[i].rdBuf.buf = &rbuf[0] + i * TY_BUFSIZE;
    ty_devs[i].wrtBuf.buf = &wbuf[0] + i * TY_BUFSIZE;
    pChan->pDrvFuncs->getTxChar = tyITx;
    pChan->pDrvFuncs->putRcvChar = tyIRd;

    return OK;

}

/**
 ******************************************************************************
 * @brief      TTY ioctl函数
 * @param[in]  uint32_t fd      ：tty句柄
 * @param[in]  int32_t request  : 控制码
 * @param[in]  uint32_t arg     : 参数
 * @retval     OK, or ERROR
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
status_t ttyIoctl(uint32_t fd, uint32_t request, uint32_t arg)
{
    if (fd > TY_NUM || fd < 1) return ERROR;
    if (ty_devs[fd-1].fd == 0) return ERROR;

    if(request == SIO_FIOFLUSH){
        semTake(ty_devs[fd-1].rdSyncSem, WAIT_FOREVER);
        ty_devs[fd-1].rdBuf.pFromBuf = ty_devs[fd-1].rdBuf.pToBuf;
        semGive(ty_devs[fd-1].rdSyncSem);
        return OK;
    }
    if (request == SIO_FIONREAD){
        uint32_t tobuf = ty_devs[fd-1].rdBuf.pToBuf;
        semTake (ty_devs[fd-1].rdSyncSem, WAIT_FOREVER );
        //未饶尾的情况
        if(ty_devs[fd-1].rdBuf.pFromBuf <= tobuf ){
          *((uint32_t*)arg) = tobuf - ty_devs[fd-1].rdBuf.pFromBuf;
        }
        //饶尾的情况
        else{
         *((uint32_t*)arg)  = tobuf + ty_devs[fd-1].rdBuf.bufSize - ty_devs[fd-1].rdBuf.pFromBuf;
        }
        semGive(ty_devs[fd-1].rdSyncSem);
        return OK;
    }
#ifdef LONGBUF
    if(request == SIO_INCBUF){
        semTake (ty_devs[fd-1].rdSyncSem,WAIT_FOREVER );
        if(lrbufused == 0){//只能被一个TY使用。
            ty_devs[fd-1].rdBuf.bufSize = MAXLONGBUF;
            ty_devs[fd-1].rdBuf.buf = lrbuf;
            lrbufused = 1;
        }
        semGive(ty_devs[fd-1].rdSyncSem);
        return OK;
    }
#endif

    return ty_devs[fd-1].pSioChan->pDrvFuncs->ioctl(ty_devs[fd-1].pSioChan,
            request, (void*)arg);;

}

/**
 ******************************************************************************
 * @brief      ttyRead -读命令
 * @param[in]  uint32_t fd      :    句柄
 * @param[out] uint8_t *buffer  :    返回的缓冲区
 * @param[in]  uint32_t maxbytes:    读取的最大字节数
 * @retval     实际返回的字节数
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
uint32_t ttyRead(uint32_t fd, uint8_t *buffer, uint32_t maxbytes)
{
    uint32_t bytenum = 0 ;
    uint32_t i = 0, tobuf = 0;

    if (fd > TY_NUM || fd < 1) return 0;
    if (ty_devs[fd-1].fd == 0) return 0;

    semTake(ty_devs[fd-1].rdSyncSem, WAIT_FOREVER);
    tobuf = ty_devs[fd-1].rdBuf.pToBuf;
    //未饶尾的情况
    if(ty_devs[fd-1].rdBuf.pFromBuf <= tobuf ){
        bytenum  = tobuf - ty_devs[fd-1].rdBuf.pFromBuf;
    }
    //饶尾的情况
    else{
        bytenum  = tobuf + ty_devs[fd-1].rdBuf.bufSize - ty_devs[fd-1].rdBuf.pFromBuf;
    }
    for(i = 0; i <MIN(bytenum,maxbytes); i++){
        *buffer = ty_devs[fd-1].rdBuf.buf[ty_devs[fd-1].rdBuf.pFromBuf];
        buffer++;
        ty_devs[fd-1].rdBuf.pFromBuf++;
        if (ty_devs[fd-1].rdBuf.pFromBuf >= ty_devs[fd-1].rdBuf.bufSize )
            ty_devs[fd-1].rdBuf.pFromBuf = 0;
    }
    semGive(ty_devs[fd-1].rdSyncSem);
    return MIN(bytenum,maxbytes);

}

/**
 ******************************************************************************
 * @brief      ttyWrite -写命令
 * @param[in]  uint32_t fd      :    句柄
 * @param[in]  uint8_t *buffer  :    写入的缓冲区
 * @param[in]  uint32_t nbytes  :    写入的字节数
 * @retval     实际写入的字节数
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
uint32_t ttyWrite (uint32_t fd, uint8_t *buffer, uint32_t nbytes)
{
    uint32_t bytenum = 0;
    uint32_t i = 0, frombuf = 0;
    uint32_t sendnum = nbytes;
    if (fd > TY_NUM || fd < 1) return 0;
    if (ty_devs[fd - 1].fd == 0) return 0;

    semTake(ty_devs[fd - 1].wrtSyncSem, WAIT_FOREVER);

    while (nbytes != 0)
    {
        frombuf = ty_devs[fd - 1].wrtBuf.pFromBuf;
        //未饶尾的情况
        if (ty_devs[fd - 1].wrtBuf.pToBuf < frombuf)
        {
            bytenum = frombuf - ty_devs[fd - 1].wrtBuf.pToBuf - 1;
        }
        //饶尾的情况
        else
        {
            bytenum = frombuf + ty_devs[fd - 1].wrtBuf.bufSize - 1
                    - ty_devs[fd - 1].wrtBuf.pToBuf;
        }
        for (i = 0; i < MIN(bytenum, nbytes); i++)
        {
            ty_devs[fd - 1].wrtBuf.buf[ty_devs[fd - 1].wrtBuf.pToBuf] = *buffer;
            buffer++;
            ty_devs[fd - 1].wrtBuf.pToBuf++;
            if (ty_devs[fd - 1].wrtBuf.pToBuf >= ty_devs[fd - 1].wrtBuf.bufSize)
                ty_devs[fd - 1].wrtBuf.pToBuf = 0;
        }
        nbytes -= MIN(bytenum, nbytes);
        ty_devs[fd - 1].pSioChan->pDrvFuncs->txStartup(ty_devs[fd - 1].pSioChan);
        if (nbytes != 0)
            taskDelay(1);
    }
    semGive(ty_devs[fd - 1].wrtSyncSem);

    return sendnum;
}

/**
 ******************************************************************************
 * @brief      ttyOpen -打开串口
 * @param[in]  uint8* name  设备名称
 * @retval     返回句柄 如果打开失败返回 0
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
uint32_t ttyOpen (uint8_t* name)
{
    uint32_t i ;

    for(i = 0 ; i < TY_NUM ; i++)
    {
        if(memcmp(ty_devs[i].name , name, 5) == 0)
        {
            //if(ty_devs[i].pSioChan->pDrvFuncs->ioctl == NULL) return 0;
            if(ty_devs[i].pSioChan->pDrvFuncs->ioctl(ty_devs[i].pSioChan, SIO_OPEN,0) == OK)
            {
                return ty_devs[i].fd;
            }
            else
            {
                return 0;
            }
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief      ttyClose -关闭串口
 * @param[in]  uint8* name  设备名称
 * @retval     返回句柄 如果打开失败返回 -1
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
status_t  ttyClose (uint32_t fd)
{
    if(fd > TY_NUM || fd < 1) return ERROR;
    return ty_devs[fd-1].pSioChan->pDrvFuncs->ioctl(ty_devs[fd-1].pSioChan, SIO_HUP, 0);
}

/**
 ******************************************************************************
 * @brief      tyITx -中断级别的输出
 * @param[in]  uint8* ttyno     : tty 号
 * @param[out] uint8_t *pChar   :
 * @retval     OK       成功
 * @retval     ERROR    失败
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
static int32_t tyITx(uint32_t ttyno,  uint8_t *pChar)
{
    if(ttyno >= TY_NUM) return ERROR;
    if(ty_devs[ttyno].fd == 0) return ERROR;
    if(ty_devs[ttyno].wrtBuf.pFromBuf == ty_devs[ttyno].wrtBuf.pToBuf)
        return ERROR;
    *pChar = ty_devs[ttyno].wrtBuf.buf[ty_devs[ttyno].wrtBuf.pFromBuf];
    ty_devs[ttyno].wrtBuf.pFromBuf++;
    if(ty_devs[ttyno].wrtBuf.pFromBuf >= ty_devs[ttyno].wrtBuf.bufSize)
        ty_devs[ttyno].wrtBuf.pFromBuf = 0;
    return OK;

}

/**
 ******************************************************************************
 * @brief      tyIRd -中断级别的输入
 * @param[in]  uint8* ttyno     :   tty 号
 * @param[in]  uint8_t   inchar :
 * @retval     OK       成功
 * @retval     ERROR    失败
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
static int32_t tyIRd(uint32_t ttyno, uint8_t inchar )
{
    if(ttyno >= TY_NUM) return ERROR;
    if(ty_devs[ttyno].fd == 0) return ERROR;
    //if(ty_devs[ttyno].rdBuf.pToBuf == ty_devs[ttyno].rdBuf.pFromBuf)
    //  return ERROR;
    ty_devs[ttyno].rdBuf.buf[ty_devs[ttyno].rdBuf.pToBuf] = inchar;
    ty_devs[ttyno].rdBuf.pToBuf ++ ;
    if(ty_devs[ttyno].rdBuf.pToBuf >= ty_devs[ttyno].rdBuf.bufSize)
        ty_devs[ttyno].rdBuf.pToBuf = 0;

    return OK;

}

/**
 ******************************************************************************
 * @brief      ttyGet -获取TTY设备
 * @param[in]  int32_t i  tty 号
 * @retval     TTY设备
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
TY_DEV* ttyGet(int32_t i)
{
    if(ty_devs[i].fd != 0){
        return &ty_devs[i];
    }
    else
        return NULL;

}

/******************************End of ttylib.c ******************************/
