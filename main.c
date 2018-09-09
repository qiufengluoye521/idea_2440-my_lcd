#include "s3c24xx.h"
#include "lcdlib.h"
#include "serial.h"

int main()
{
    uart_init();
    putc('m');
    putc('a');
    putc('i');
    putc('n');
    putc('\n');
    Test_Lcd_Tft_16Bit_480272();   
    return 1;
}
