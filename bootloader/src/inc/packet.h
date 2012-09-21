/**
 * @file   : packet.h
 * Fuction : CM3 bootloader
 * By      : LiuNing
 * Date    : 2012-3-10
 */

#ifndef _PACKET_H_
#define _PACKET_H_

#include "cfg.h"

// ������
#define COMMAND_PING             0x20    //!< 0x20 ��������
#define COMMAND_DOWNLOAD         0x21    //!< 0x21 ��ʼ����
#define COMMAND_GET_STATUS       0x23    //!< 0x23 ��ȡ״̬
#define COMMAND_SEND_DATA        0x24    //!< 0x24 ��������
#define COMMAND_RESET            0x25    //!< 0x25 ��λ

// Ӧ����
#define COMMAND_ACK              0xCC    //!< 0xCC ���ճɹ�
#define COMMAND_NAK              0x33    //!< 0x33 ����ʧ��

// ״̬��
#define COMMAND_RET_SUCCESS      0x40    //!< 0x40 �ɹ�
#define COMMAND_RET_UNKNOWN_CMD  0x41    //!< 0x41 δ֪����
#define COMMAND_RET_INVALID_CMD  0x42    //!< 0x42 �Ƿ�����
#define COMMAND_RET_INVALID_PARA 0x43    //!< 0x43 �Ƿ�����
#define COMMAND_RET_FLASH_FAIL   0x44    //!< 0x44 FLASH����ʧ��
#define COMMAND_RET_BUFFER_FAIL  0x45    //!< 0x45 ����������

/*------------------------------------------------------------------------------
Section: Function Prototypes
------------------------------------------------------------------------------*/
/***************************************************************************//**
*
* packet_hello - ����ͨ�������˶��ఴ���򴮿��л������Ĵ����ֽ�
*
* @param void
*
* @retval true	�ѽ���
* @retval false δ����
*
*******************************************************************************/
extern boolean packet_hello(void);

/***************************************************************************//**
*
* packet_receive - ����һ�����ݰ�
*
* @param [in] cmd  ������
* @param [in] data ���ջ���
* @param [in] size data����
*
* @return У��ֵ
*
*******************************************************************************/
extern boolean packet_receive(uint8 *cmd, uint8 *data, uint8 *size);

/***************************************************************************//**
*
* packet_status - ���͵�ǰ״̬
*
* @param [in] s ״̬
*
* @retval true	���ͳɹ�
* @retval false ����ʧ��
*
*******************************************************************************/
extern boolean packet_status(uint8 s);

/***************************************************************************//**
*
* packet_ack - ���ؽ���ȷ�ϰ�
*
* @param void
*
* @return void
*
* @note ע��:ֻ˵�����ݰ��Ϸ����ѱ�����������ζ��ҵ��������Ҳ��ȷ
*
*******************************************************************************/
extern void packet_ack(void);

/***************************************************************************//**
*
* packet_nak - ���ؽ��շ��ϰ�
*
* @param void
*
* @return void
*
*******************************************************************************/
extern void packet_nak(void);

#endif /* _PACKET_H_ */

/*-------------------------------End of packet.h------------------------------*/
