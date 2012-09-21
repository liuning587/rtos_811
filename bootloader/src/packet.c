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

// ȷ�ϰ�
static const uint8 PACKET_ACK[] = {0, 0, COMMAND_ACK };
// ���ϰ�
static const uint8 PACKET_NAK[] = {0, 0, COMMAND_NAK };

static boolean checkSum(const uint8 cmd, const uint8 *data,
                        const uint8 size, const uint8 sum);
static void send(const uint8 *data, uint16 size);

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
void packet_ack(void)
{
    send(PACKET_ACK, ARRAY_SIZE(PACKET_ACK));
}

/***************************************************************************//**
*
* packet_nak - ���ؽ��շ��ϰ�
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
* packet_hello - ����ͨ�������˶��ఴ���򴮿��л������Ĵ����ֽ�
*
* @param void
*
* @retval true  �ѽ���
* @retval false δ����
*
*******************************************************************************/
boolean packet_hello(void)
{
    uint8 data;

    // ��ȡ֡�����ֽ�
    do
    {
        uart_receive(&data, 1);
    }
    while (3 != data);   // �������֡���Ȳ�Ϊ3�������

    // ��ȡ֡У���ֽ�
    uart_receive(&data, 1);
    if (data != 0x20) return (false);

    // ��ȡ֡������
    uart_receive(&data, 1);
    if (data != 0x20) return (false);

    // ����ȷ�ϰ�
    packet_ack();

    return (true);
}

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
boolean packet_receive(uint8 *cmd, uint8 *data, uint8 *size)
{
    uint8 sum;

    // ��ȡ֡�����ֽ�
    do
    {
        uart_receive(size, 1);
    }
    while (*size < 3);   // ֡���� >= 3

    // ��ȡ֡У���ֽ�
    uart_receive(&sum, 1);

    // ��ȡ������
    uart_receive(cmd, 1);

    // �۳����ȡ������ּ�У���ֽڣ������ȡ���ֽ���
    *size -= 3;

    if (*size > 0)
        uart_receive(data, *size);

    // ���֡У����󣬷���false
    if (!checkSum(*cmd, data, *size, sum))
        return (false);


    return (true);
}

/***************************************************************************//**
*
* packet_status - ���͵�ǰ״̬
*
* @param [in] s ״̬
*
* @retval true  ���ͳɹ�
* @retval false ����ʧ��
*
*******************************************************************************/
boolean packet_status(uint8 s)
{
    uint8 data;

    // ���ͳ����ֽ�
    data = 3;
    send((unsigned char *)&data, 1);

    // ����У���ֽ�
    send((unsigned char *)&s, 1);

    // ����״̬�ֽ�
    send((unsigned char *)&s, 1);

    // �ȴ�����һ����0�ֽ�
    do
    {
        uart_receive((uint8 *)&data, 1);
    }
    while (0 == data);

    // ���δ����ACK������Ϊ����ʧ��
    if (data == COMMAND_ACK)
        return (true);
    else return (false);
}

/***************************************************************************//**
*
* send - ��������
*
* @param [in] *data ����������ʼָ��
* @param [in] size  �����ֽ���
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
* checkSum - ���У��ֵ
*
* @param [in] cmd  ������
* @param [in] data ����ָ��
* @param [in] size ���ݳ���
* @param [in] sum  У��ֵ
*
* @retval true  ��ȷ
* @retval flase ����
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
