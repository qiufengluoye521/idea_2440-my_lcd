#include "lcdlib.h"
#include "lcddrv.h"
#include "framebuffer.h"

/* 
 * ��480x272,16bpp����ʾģʽ����TFT LCD
 */
void Test_Lcd_Tft_16Bit_480272(void)
{
    Lcd_Port_Init();                     // ����LCD����
    Tft_Lcd_Init(MODE_TFT_16BIT_480272); // ��ʼ��LCD������
    //Lcd_PowerEnable(0, 1);               // ����LCD_PWREN��Ч�������ڴ�LCD�ĵ�Դ
    Lcd_EnvidOnOff(1);                   // ʹ��LCD����������ź�

    ClearScr(0x001F);  // ��������ɫ
    
    DrawLine(0  , 0  , 479, 0  , 0xff0000);    // ��ɫ
    DrawLine(0  , 0  , 0  , 271, 0x00ff00);    // ��ɫ
    DrawLine(479, 0  , 479, 271, 0x0000ff);    // ��ɫ
    DrawLine(0  , 271, 479, 271, 0xffffff);    // ��ɫ
    DrawLine(0  , 0  , 479, 271, 0xffff00);    // ��ɫ
    DrawLine(479, 0  , 0  , 271, 0x8000ff);    // ��ɫ
    DrawLine(240, 0  , 240, 271, 0xe6e8fa);    // ��ɫ
    DrawLine(0  , 136, 479, 136, 0xcd7f32);    // ��ɫ
}
