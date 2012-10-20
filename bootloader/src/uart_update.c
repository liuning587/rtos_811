/**
 * @file   : uart_update.c
 * Fuction : CM3 bootloader
 * By      : LiuNing
 * Date    : 2012-3-10
 */

#include "./inc/lib.h"
#include "./inc/lm3s.h"
#include "./inc/packet.h"
#include "./inc/iflash.h"
#include "./inc/uart.h"
#include "./inc/cfg.h"

extern boolean escKeyDetect(void);

/***************************************************************************//**
*
* uart_update - 从串口进行升级
*
* @param void
*
* @retval true  成功
* @retval false 失败
*
*******************************************************************************/
STATUS uart_update(void)
{
    uint32 i, startAddress, transferSize = 0;
    uint8 size;
    uint8 status;
    uint8 cmd, data[MAX_BUF_SIZE];

    uart_printf("Press ESC to upgrade from serial port ");

    // 检测是否要从串口进行升级
    if (!escKeyDetect()) return (OK);

    uart_printf("Receiving data ..");

    // 过滤多余按键或串口切换产生的错误字节
    while (!packet_hello())
    {
    }

    startAddress = 0xffffffff;

    status = COMMAND_RET_SUCCESS;
    while (true)
    {
        // 如果未正确接收报文，放弃处理
        if (!packet_receive(&cmd, data, &size))
        {
            packet_nak();
            continue;
        }

        // 根据命令字进行处理
        switch(cmd)
        {
        case COMMAND_PING: // 测试连接
            status = COMMAND_RET_SUCCESS;

            // 返回确认包
            packet_ack();

            break;

        case COMMAND_DOWNLOAD: // 开始下载
            status = COMMAND_RET_SUCCESS;
            do
            {
                // 检查报文长度
                if (size != 8)
                {
                    status = COMMAND_RET_INVALID_CMD;

                    break;
                }

                // 起始地址
                startAddress = (data[0] << 24)
                               | (data[1] << 16)
                               | (data[2] <<  8)
                               |  data[3];
                /* 起始地址必须为0，由各BootLoader自行决定写入位置 */
                if (startAddress != 0u)
                {
                    status = COMMAND_RET_INVALID_CMD;
                    break;
                }
                startAddress = APP_START_ADDRESS;
                // 下载字节数
                transferSize = (data[4] << 24)
                               | (data[5] << 16)
                               | (data[6] <<  8)
                               |  data[7];

                // 检查写入空间
                if (!iflash_spaceCheck(startAddress, transferSize))
                {
                    status = COMMAND_RET_INVALID_PARA;

                    break;
                }

                // 清除中断状态
                iflash_errorClear();

                // 按块擦除将要更新的地址区间
                for (i = startAddress; i < startAddress + transferSize; i += FLASH_PAGE_SIZE)
                    iflash_erase(i);

                // 检查中断状态，如果出现错误，则设置对应状态
                if (iflash_errorCheck())
                    status = COMMAND_RET_FLASH_FAIL;
            }
            while(false);

            if (status != COMMAND_RET_SUCCESS)
                transferSize = 0;

            // 确认报文已得到处理
            packet_ack();

            break;

        case COMMAND_GET_STATUS: // 获取状态
            // 返回确认包
            packet_ack();

            // 返回当前状态
            packet_status(status);

            break;

        case COMMAND_SEND_DATA: // 数据
            status = COMMAND_RET_SUCCESS;

            // 注意:后台程序应保证size按字对齐，否则写入结果无法预料
            // 检查是否还有数据未写入
            if (transferSize >= size)
            {
                iflash_write(startAddress, data, size);
                if (iflash_errorCheck())
                    status = COMMAND_RET_FLASH_FAIL;
                else
                {
                    transferSize -= size;
                    startAddress += size;
                }
            }
            else status = COMMAND_RET_INVALID_PARA;

            // 返回确认包
            packet_ack();

            break;

        case COMMAND_RESET: // 复位
            // 返回确认包
            packet_ack();

            // 等待UART完成发送
            uart_flush();

            delay(524288);

            // 写入注册码及复位请求
            HWREG(NVIC_APINT) = (NVIC_APINT_VECTKEY | NVIC_APINT_SYSRESETREQ);

            // 死循环
            while(true)
            {
            }
        default: // 未知命令
            // 返回确认包
            packet_ack();

            // 状态设为未知命令
            status = COMMAND_RET_UNKNOWN_CMD;
        }
    }
}

/*-------------------------------End of check.c-------------------------------*/
