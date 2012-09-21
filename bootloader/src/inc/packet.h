/**
 * @file   : packet.h
 * Fuction : CM3 bootloader
 * By      : LiuNing
 * Date    : 2012-3-10
 */

#ifndef _PACKET_H_
#define _PACKET_H_

#include "cfg.h"

// 命令码
#define COMMAND_PING             0x20    //!< 0x20 测试连接
#define COMMAND_DOWNLOAD         0x21    //!< 0x21 开始下载
#define COMMAND_GET_STATUS       0x23    //!< 0x23 获取状态
#define COMMAND_SEND_DATA        0x24    //!< 0x24 发送数据
#define COMMAND_RESET            0x25    //!< 0x25 复位

// 应答码
#define COMMAND_ACK              0xCC    //!< 0xCC 接收成功
#define COMMAND_NAK              0x33    //!< 0x33 接收失败

// 状态码
#define COMMAND_RET_SUCCESS      0x40    //!< 0x40 成功
#define COMMAND_RET_UNKNOWN_CMD  0x41    //!< 0x41 未知命令
#define COMMAND_RET_INVALID_CMD  0x42    //!< 0x42 非法命令
#define COMMAND_RET_INVALID_PARA 0x43    //!< 0x43 非法参数
#define COMMAND_RET_FLASH_FAIL   0x44    //!< 0x44 FLASH操作失败
#define COMMAND_RET_BUFFER_FAIL  0x45    //!< 0x45 缓冲区不足

/*------------------------------------------------------------------------------
Section: Function Prototypes
------------------------------------------------------------------------------*/
/***************************************************************************//**
*
* packet_hello - 建立通道，过滤多余按键或串口切换产生的错误字节
*
* @param void
*
* @retval true	已建立
* @retval false 未建立
*
*******************************************************************************/
extern boolean packet_hello(void);

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
extern boolean packet_receive(uint8 *cmd, uint8 *data, uint8 *size);

/***************************************************************************//**
*
* packet_status - 发送当前状态
*
* @param [in] s 状态
*
* @retval true	发送成功
* @retval false 发送失败
*
*******************************************************************************/
extern boolean packet_status(uint8 s);

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
extern void packet_ack(void);

/***************************************************************************//**
*
* packet_nak - 返回接收否认包
*
* @param void
*
* @return void
*
*******************************************************************************/
extern void packet_nak(void);

#endif /* _PACKET_H_ */

/*-------------------------------End of packet.h------------------------------*/
