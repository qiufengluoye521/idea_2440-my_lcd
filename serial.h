#ifndef __SERIAL_H_
#define __SERIAL_H_

void uart_init(void);
void putc(unsigned char c);
unsigned char getc(void);

#endif              // __SERIAL_H_
