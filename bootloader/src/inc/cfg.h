/**
 * @file   : cfg.h
 * Fuction : bootloader�����ļ�.
 * By      : LiuNing
 * Date    : 2012-3-10
 */

#ifndef _CFG_H_
#define _CFG_H_

#define CRYSTAL_FREQ         6000000      //!< ʱ��Ƶ��
#define STACK_SIZE           512          //!< ջ��С(��λΪ��)
#define MAX_BUF_SIZE         252          //!< ���ݻ�������С��ע��:���� <= 252
#define FLASH_PAGE_SIZE      0x00000400   //!< Ƭ��FLASHҳ��С
#define APP_START_ADDRESS    0x00000800   //!< Ӧ�ó�����ʼ��ַ
#define UART_BAUDRATE        115200       //!< ���ڲ�����
#define MAX_WAITKEY_TIME     5            //!< �ȴ�����ʱ��


// ȷ��Ӧ�ó����ַ��ҳ����
#if (APP_START_ADDRESS & (FLASH_PAGE_SIZE - 1))
#error ERROR: APP_START_ADDRESSȡֵ����Ӧ��ҳ����
#endif

#define OK		          0
#define ERROR          (-1)

#define BOOL           int8
#define IMPORT       extern
#define LOCAL        static
#define FAST       register
#define NULL              0
#define TRUE              1
#define FALSE             0

typedef char                int8;
typedef short int           int16;
typedef long int            int32;
typedef long long		    int64;

typedef unsigned char       uint8;
typedef unsigned short int  uint16;
typedef unsigned long int   uint32;
typedef unsigned long long  uint64;
typedef	int		STATUS;

typedef enum  Boolean
{
    false = 0,
    true  = 1
} boolean;

#define MAX(x, y)       (((x) < (y)) ? (y) : (x)) // ȡ��ֵ
#define MIN(x, y)       (((x) < (y)) ? (x) : (y)) // ȡСֵ
#define BCD2HEX(x)      (((x) >> 4) * 10 + ((x) & 0x0F))        /* 20H -> 20*/
#define HEX2BCD(x)      (((x) % 10) + ((((x) / 10) % 10) << 4)) /* 20 -> 20H*/
#define LMMSB(x)        (((x)>>32)&0xff)
#define LLLSB(x)        ((x) & 0x0F)
#define LLNLSB(x)       (((x) >> 4) & 0x0F)
#define LLNLHB(x)       (((x) >> 8) & 0x0f)
#define BYTESWAP(x)     ((MSB((x)) | (LSB((x)) << 8)))
#define BITS(x,y)       (((x)>>(y))&0x01)   /* �ж�ĳλ�Ƿ�Ϊ1 */
#define SETBITS(x,y,n)  (x) = (n) ? ((x)|(1 << (y))) : ((x) &(~(1 << (y))));
#define INVERSE(x,y)    ((x)=(x)^(1<<(y)))  /* ��ĳλ�÷� */
#define ARRAY_SIZE(a)   (sizeof (a) / sizeof ((a)[0]))

#define OFFSET(structure, member)	/* byte offset of member in structure*/\
		((int)&(((structure *) 0) -> member))

#define MEMBER_SIZE(structure, member)	/* size of a member of a structure */\
		(sizeof (((structure *) 0) -> member))


#endif /* _CFG_H_ */

