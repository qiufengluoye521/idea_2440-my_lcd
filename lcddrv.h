#ifndef __LCDDRV_H
#define __LCDDRV_H

#define LOWER21BITS(n)  ((n) & 0x1fffff)
#define LCDFRAMEBUFFER  0x30400000

#define BPPMODE_16BPP   0xC

#define LCDTYPE_TFT     0x3
#define ENVID_DISABLE   0
#define ENVID_ENABLE    1
#define FORMAT8BPP_5551 0
#define FORMAT8BPP_565  1
#define HSYNC_NORM      0
#define HSYNC_INV       1

#define VSYNC_NORM      0
#define VSYNC_INV       1

#define VDEN_NORM       0
#define VDEN_INV        1

#define BSWP            1
#define HWSWP           1

#define LCDFRAMEBUFFER 0x30400000

#define MODE_TFT_16BIT_480272    (0x4110)


void Lcd_Port_Init(void);
void Tft_Lcd_Init(int type);
void Lcd_EnvidOnOff(int onoff);


#endif      //__LCDDRV_H
