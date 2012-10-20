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
* uart_update - �Ӵ��ڽ�������
*
* @param void
*
* @retval true  �ɹ�
* @retval false ʧ��
*
*******************************************************************************/
STATUS uart_update(void)
{
    uint32 i, startAddress, transferSize = 0;
    uint8 size;
    uint8 status;
    uint8 cmd, data[MAX_BUF_SIZE];

    uart_printf("Press ESC to upgrade from serial port ");

    // ����Ƿ�Ҫ�Ӵ��ڽ�������
    if (!escKeyDetect()) return (OK);

    uart_printf("Receiving data ..");

    // ���˶��ఴ���򴮿��л������Ĵ����ֽ�
    while (!packet_hello())
    {
    }

    startAddress = 0xffffffff;

    status = COMMAND_RET_SUCCESS;
    while (true)
    {
        // ���δ��ȷ���ձ��ģ���������
        if (!packet_receive(&cmd, data, &size))
        {
            packet_nak();
            continue;
        }

        // ���������ֽ��д���
        switch(cmd)
        {
        case COMMAND_PING: // ��������
            status = COMMAND_RET_SUCCESS;

            // ����ȷ�ϰ�
            packet_ack();

            break;

        case COMMAND_DOWNLOAD: // ��ʼ����
            status = COMMAND_RET_SUCCESS;
            do
            {
                // ��鱨�ĳ���
                if (size != 8)
                {
                    status = COMMAND_RET_INVALID_CMD;

                    break;
                }

                // ��ʼ��ַ
                startAddress = (data[0] << 24)
                               | (data[1] << 16)
                               | (data[2] <<  8)
                               |  data[3];
                /* ��ʼ��ַ����Ϊ0���ɸ�BootLoader���о���д��λ�� */
                if (startAddress != 0u)
                {
                    status = COMMAND_RET_INVALID_CMD;
                    break;
                }
                startAddress = APP_START_ADDRESS;
                // �����ֽ���
                transferSize = (data[4] << 24)
                               | (data[5] << 16)
                               | (data[6] <<  8)
                               |  data[7];

                // ���д��ռ�
                if (!iflash_spaceCheck(startAddress, transferSize))
                {
                    status = COMMAND_RET_INVALID_PARA;

                    break;
                }

                // ����ж�״̬
                iflash_errorClear();

                // ���������Ҫ���µĵ�ַ����
                for (i = startAddress; i < startAddress + transferSize; i += FLASH_PAGE_SIZE)
                    iflash_erase(i);

                // ����ж�״̬��������ִ��������ö�Ӧ״̬
                if (iflash_errorCheck())
                    status = COMMAND_RET_FLASH_FAIL;
            }
            while(false);

            if (status != COMMAND_RET_SUCCESS)
                transferSize = 0;

            // ȷ�ϱ����ѵõ�����
            packet_ack();

            break;

        case COMMAND_GET_STATUS: // ��ȡ״̬
            // ����ȷ�ϰ�
            packet_ack();

            // ���ص�ǰ״̬
            packet_status(status);

            break;

        case COMMAND_SEND_DATA: // ����
            status = COMMAND_RET_SUCCESS;

            // ע��:��̨����Ӧ��֤size���ֶ��룬����д�����޷�Ԥ��
            // ����Ƿ�������δд��
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

            // ����ȷ�ϰ�
            packet_ack();

            break;

        case COMMAND_RESET: // ��λ
            // ����ȷ�ϰ�
            packet_ack();

            // �ȴ�UART��ɷ���
            uart_flush();

            delay(524288);

            // д��ע���뼰��λ����
            HWREG(NVIC_APINT) = (NVIC_APINT_VECTKEY | NVIC_APINT_SYSRESETREQ);

            // ��ѭ��
            while(true)
            {
            }
        default: // δ֪����
            // ����ȷ�ϰ�
            packet_ack();

            // ״̬��Ϊδ֪����
            status = COMMAND_RET_UNKNOWN_CMD;
        }
    }
}

/*-------------------------------End of check.c-------------------------------*/
