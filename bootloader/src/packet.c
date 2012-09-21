/**
 * @file   : packet.c
 * Fuction : CM3 bootloader
 * By      : LiuNing
 * Date    : 2012-3-10
 */

#include "./inc/lib.h"
#include "./inc/cfg.h"
#include "./inc/packet.h"
#include "./inc/uart.h"

// 确认包
static const uint8 PACKET_ACK[] = {0, 0, COMMAND_ACK };
// 否认包
static const uint8 PACKET_NAK[] = {0, 0, COMMAND_NAK };

static boolean checkSum(const uint8 cmd, const uint8 *data,
                        const uint8 size, const uint8 sum);
static void send(const uint8 *data, uint16 size);

/***************************************************************************//**
*
* packet_ack - 返回接收确认包
*
* @param void
*
* @return void
*
* @note 注意:只说明数据包合法并已被处理，而不意味着业务流程上也正确
*
*******************************************************************************/
void packet_ack(void)
{
    send(PACKET_ACK, ARRAY_SIZE(PACKET_ACK));
}

/***************************************************************************//**
*
* packet_nak - 返回接收否认包
*
* @param void
*
* @return void
*
*******************************************************************************/
void packet_nak(void)
{
    send(PACKET_NAK, ARRAY_SIZE(PACKET_NAK));
}

/***************************************************************************//**
*
* packet_hello - 建立通道，过滤多余按键或串口切换产生的错误字节
*
* @param void
*
* @retval true  已建立
* @retval false 未建立
*
*******************************************************************************/
boolean packet_hello(void)
{
    uint8 data;

    // 读取帧长度字节
    do
    {
        uart_receive(&data, 1);
    }
    while (3 != data);   // 如果返回帧长度不为3，则放弃

    // 读取帧校验字节
    uart_receive(&data, 1);
    if (data != 0x20) return (false);

    // 读取帧命令字
    uart_receive(&data, 1);
    if (data != 0x20) return (false);

    // 返回确认包
    packet_ack();

    return (true);
}

/***************************************************************************//**
*
* packet_receive - 接收一个数据包
*
* @param [in] cmd  命令字
* @param [in] data 接收缓冲
* @param [in] size data长度
*
* @return 校验值
*
*******************************************************************************/
boolean packet_receive(uint8 *cmd, uint8 *data, uint8 *size)
{
    uint8 sum;

    // 读取帧长度字节
    do
    {
        uart_receive(size, 1);
    }
    while (*size < 3);   // 帧长度 >= 3

    // 读取帧校验字节
    uart_receive(&sum, 1);

    // 读取命令字
    uart_receive(cmd, 1);

    // 扣除长度、命令字及校验字节，还需读取的字节数
    *size -= 3;

    if (*size > 0)
        uart_receive(data, *size);

    // 如果帧校验错误，返回false
    if (!checkSum(*cmd, data, *size, sum))
        return (false);


    return (true);
}

/***************************************************************************//**
*
* packet_status - 发送当前状态
*
* @param [in] s 状态
*
* @retval true  发送成功
* @retval false 发送失败
*
*******************************************************************************/
boolean packet_status(uint8 s)
{
    uint8 data;

    // 发送长度字节
    data = 3;
    send((unsigned char *)&data, 1);

    // 发送校验字节
    send((unsigned char *)&s, 1);

    // 发送状态字节
    send((unsigned char *)&s, 1);

    // 等待返回一个非0字节
    do
    {
        uart_receive((uint8 *)&data, 1);
    }
    while (0 == data);

    // 如果未返回ACK，则认为发送失败
    if (data == COMMAND_ACK)
        return (true);
    else return (false);
}

/***************************************************************************//**
*
* send - 发送数据
*
* @param [in] *data 发送数据起始指针
* @param [in] size  发送字节数
*
* @return void
*
*******************************************************************************/
static void send(const uint8 *data, uint16 size)
{
    uint16 i;

    for (i = 0; i < size; i++)
    {
        uart_send(data[i]);
    }
}

/***************************************************************************//**
*
* checkSum - 检查校验值
*
* @param [in] cmd  命令字
* @param [in] data 数据指针
* @param [in] size 数据长度
* @param [in] sum  校验值
*
* @retval true  正确
* @retval flase 错误
*
*******************************************************************************/
LOCAL boolean checkSum(const uint8 cmd, const uint8 *data,
                       const uint8 size, const uint8 sum)
{
    uint8 s = cmd;
    uint8 i = size;

    if (i > 0)
    {
        while (i--) s += *data++;
    }

    return ((s & 0xff) == sum ? true : false);
}

/*------------------------------End of packet.c-------------------------------*/
