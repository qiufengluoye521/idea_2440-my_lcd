#include "serial.h"
#include "s3c24xx.h"

#define TXD0READY       (1<<2)
#define RXD0READY       (1)

#define PCLK            50000000    // init.c�е�clock_init��������PCLKΪ50MHz
#define UART_CLK        PCLK        //  UART0��ʱ��Դ��ΪPCLK
#define UART_BAUD_RATE  115200      // ������
#define UART_BRD        ((UART_CLK  / (UART_BAUD_RATE * 16)) - 1)

void uart_init(void)
{
    GPHCON      |= 0xA0;            // GPH2,GPH3����TXD0,RXD0
    GPHUP       =  0x0c;            // GPH2,GPH3�ڲ�����
    
    ULCON0      = 0x03;             // 8N1(8������λ���޽��飬1��ֹͣλ)
    UCON0       = 0x05;             // ��ѯ��ʽ��UARTʱ��ԴΪPCLK
    UFCON0      = 0x00;             // ��ʹ��FIFO
    UMCON0      = 0x00;             // ��ʹ������
    UBRDIV0     = UART_BRD;         // ������Ϊ115200
    
}

/*
 * ����һ���ַ�
 */
void putc(unsigned char c)
{
    /* �ȴ���ֱ�����ͻ������е������Ѿ�ȫ�����ͳ�ȥ */
    while(!(UTRSTAT0 & TXD0READY));

    /* ��UTXH0�Ĵ�����д�����ݣ�UART���Զ��������ͳ�ȥ */
    UTXH0 = c;
}

/*
* �����ַ�
*/
unsigned char getc(void)
{
    /*�ȴ���ֱ�����ջ������е�������*/
    while (!(UTRSTAT0 & RXD0READY));

    return URXH0;
}
