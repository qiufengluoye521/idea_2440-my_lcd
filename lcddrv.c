//#include "stdio.h"
#include "s3c24xx.h"
#include "lcddrv.h"

#define GPB0_tout0  (2<<(0*2))
#define GPB0_out    (1<<(0*2))
#define GPB1_out    (1<<(1*2))

#define GPB0_MSK    (3<<(0*2))
#define GPB1_MSK    (3<<(1*2))

unsigned int fb_base_addr;
unsigned int bpp;
unsigned int xsize;
unsigned int ysize;


/*
 * ��ʼ������LCD������
 */
void Lcd_Port_Init(void)
{
    GPCUP   = 0xffffffff;               // ��ֹ�ڲ�����
    GPCCON  = 0xaaaaaaaa;               // GPIO�ܽ�����VD[7:0],LCDVF[2:0],VM,VFRAME,VLINE,VCLK,LEND 
    GPDUP   = 0xffffffff;               // ��ֹ�ڲ�����
    GPDCON  = 0xaaaaaaaa;               // GPIO�ܽ�����VD[23:8]
    GPBCON &= ~(GPB0_MSK);              // Power enable pin
    GPBCON |= GPB0_out;         
    GPBDAT &= ~(1<<0);                  // Power off
}

/*
 * ��ʼ��LCD������
 * ���������
 * type: ��ʾģʽ
 *      MODE_TFT_8BIT_240320  : 240*320 8bpp��TFT LCD
 *      MODE_TFT_16BIT_240320 : 240*320 16bpp��TFT LCD
 *      MODE_TFT_8BIT_640480  : 640*480 8bpp��TFT LCD
 *      MODE_TFT_16BIT_640480 : 640*480 16bpp��TFT LCD
 */
void Tft_Lcd_Init(int type)
{
    /* 
     * ����LCD�������Ŀ��ƼĴ���LCDCON1~5
     * 1. LCDCON1:
     *    ����VCLK��Ƶ�ʣ�VCLK(Hz) = HCLK/[(CLKVAL+1)x2]
     *    ѡ��LCD����: TFT LCD   
     *    ������ʾģʽ: 16BPP
     *    �Ƚ�ֹLCD�ź����
     * 2. LCDCON2/3/4:
     *    ���ÿ����źŵ�ʱ�����
     *    ���÷ֱ��ʣ�������������
     * ���ڣ����Ը��ݹ�ʽ�������ʾ����Ƶ�ʣ�
     * ��HCLK=100MHzʱ��
     * Frame Rate = 1/[{(VSPW+1)+(VBPD+1)+(LIINEVAL+1)+(VFPD+1)}x
     *              {(HSPW+1)+(HBPD+1)+(HFPD+1)+(HOZVAL+1)}x
     *              {2x(CLKVAL+1)/(HCLK)}]
     *            = 60Hz
     * 3. LCDCON5:
     *    ������ʾģʽΪ16BPPʱ�����ݸ�ʽ: 5:6:5
     *    ����HSYNC��VSYNC����ļ���(����Ҫ�ο�����LCD�Ľӿ��ź�): ��ת
     *    ����(2�ֽ�)����ʹ��
     */
    LCDCON1 = (4<<8) | (LCDTYPE_TFT<<5) | \
              (BPPMODE_16BPP<<1) | (ENVID_DISABLE<<0);
    LCDCON2 = (1<<24) | (271<<14) | \
              (1<<6) | (9);
    LCDCON3 = (1<<19) | (479<<8) | (1);
    LCDCON4 = 40;
    LCDCON5 = (FORMAT8BPP_565<<11) | (HSYNC_INV<<9) | (VSYNC_INV<<8) | \
              (HWSWP<<1);

    /*
     * ����LCD�������ĵ�ַ�Ĵ���LCDSADDR1~3
     * ֡�ڴ����ӿ�(view point)��ȫ�Ǻϣ�
     * ͼ�����ݸ�ʽ���£�
     *         |----PAGEWIDTH----|
     *    y/x  0   1   2       239
     *     0   rgb rgb rgb ... rgb
     *     1   rgb rgb rgb ... rgb
     * 1. LCDSADDR1:
     *    ����LCDBANK��LCDBASEU
     * 2. LCDSADDR2:
     *    ����LCDBASEL: ֡�������Ľ�����ַA[21:1]
     * 3. LCDSADDR3:
     *    OFFSIZE����0��PAGEWIDTH����(240*2/2)
     */
    LCDSADDR1 = ((LCDFRAMEBUFFER>>22)<<21) | LOWER21BITS(LCDFRAMEBUFFER>>1);
    LCDSADDR2 = LOWER21BITS((LCDFRAMEBUFFER+ \
                (480)*(272)*2)>>1);
    LCDSADDR3 = (0<<11) | (480*2/2);

    /* ��ֹ��ʱ��ɫ��Ĵ��� */
    TPAL = 0;

    fb_base_addr = LCDFRAMEBUFFER;
    bpp = 16;
    xsize = 480;
    ysize = 272;
}

/*
 * ����LCD�������Ƿ�����ź�
 * ���������
 * onoff: 
 *      0 : �ر�
 *      1 : ��
 */
void Lcd_EnvidOnOff(int onoff)
{
    if (onoff == 1)
    {
        LCDCON1 |= 1;           // ENVID ON
        GPBDAT |= (1<<0);       // Power on
    }
    else
    {
        LCDCON1 &= 0x3fffe;     // ENVID Off
        GPBDAT &= ~(1<<0);      // Power off
    }
}    
