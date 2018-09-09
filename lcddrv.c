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
 * 初始化用于LCD的引脚
 */
void Lcd_Port_Init(void)
{
    GPCUP   = 0xffffffff;               // 禁止内部上拉
    GPCCON  = 0xaaaaaaaa;               // GPIO管脚用于VD[7:0],LCDVF[2:0],VM,VFRAME,VLINE,VCLK,LEND 
    GPDUP   = 0xffffffff;               // 禁止内部上拉
    GPDCON  = 0xaaaaaaaa;               // GPIO管脚用于VD[23:8]
    GPBCON &= ~(GPB0_MSK);              // Power enable pin
    GPBCON |= GPB0_out;         
    GPBDAT &= ~(1<<0);                  // Power off
}

/*
 * 初始化LCD控制器
 * 输入参数：
 * type: 显示模式
 *      MODE_TFT_8BIT_240320  : 240*320 8bpp的TFT LCD
 *      MODE_TFT_16BIT_240320 : 240*320 16bpp的TFT LCD
 *      MODE_TFT_8BIT_640480  : 640*480 8bpp的TFT LCD
 *      MODE_TFT_16BIT_640480 : 640*480 16bpp的TFT LCD
 */
void Tft_Lcd_Init(int type)
{
    /* 
     * 设置LCD控制器的控制寄存器LCDCON1~5
     * 1. LCDCON1:
     *    设置VCLK的频率：VCLK(Hz) = HCLK/[(CLKVAL+1)x2]
     *    选择LCD类型: TFT LCD   
     *    设置显示模式: 16BPP
     *    先禁止LCD信号输出
     * 2. LCDCON2/3/4:
     *    设置控制信号的时间参数
     *    设置分辨率，即行数及列数
     * 现在，可以根据公式计算出显示器的频率：
     * 当HCLK=100MHz时，
     * Frame Rate = 1/[{(VSPW+1)+(VBPD+1)+(LIINEVAL+1)+(VFPD+1)}x
     *              {(HSPW+1)+(HBPD+1)+(HFPD+1)+(HOZVAL+1)}x
     *              {2x(CLKVAL+1)/(HCLK)}]
     *            = 60Hz
     * 3. LCDCON5:
     *    设置显示模式为16BPP时的数据格式: 5:6:5
     *    设置HSYNC、VSYNC脉冲的极性(这需要参考具体LCD的接口信号): 反转
     *    半字(2字节)交换使能
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
     * 设置LCD控制器的地址寄存器LCDSADDR1~3
     * 帧内存与视口(view point)完全吻合，
     * 图像数据格式如下：
     *         |----PAGEWIDTH----|
     *    y/x  0   1   2       239
     *     0   rgb rgb rgb ... rgb
     *     1   rgb rgb rgb ... rgb
     * 1. LCDSADDR1:
     *    设置LCDBANK、LCDBASEU
     * 2. LCDSADDR2:
     *    设置LCDBASEL: 帧缓冲区的结束地址A[21:1]
     * 3. LCDSADDR3:
     *    OFFSIZE等于0，PAGEWIDTH等于(240*2/2)
     */
    LCDSADDR1 = ((LCDFRAMEBUFFER>>22)<<21) | LOWER21BITS(LCDFRAMEBUFFER>>1);
    LCDSADDR2 = LOWER21BITS((LCDFRAMEBUFFER+ \
                (480)*(272)*2)>>1);
    LCDSADDR3 = (0<<11) | (480*2/2);

    /* 禁止临时调色板寄存器 */
    TPAL = 0;

    fb_base_addr = LCDFRAMEBUFFER;
    bpp = 16;
    xsize = 480;
    ysize = 272;
}

/*
 * 设置LCD控制器是否输出信号
 * 输入参数：
 * onoff: 
 *      0 : 关闭
 *      1 : 打开
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
