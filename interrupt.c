#include "s3c24xx.h"

#define	GPX5_out	(1<<(5*2))

// ����ָ������
void (*isr_handle_array[50])(void);

void Dummy_isr(void)
{
    while(1);
}

void  wait_irq(volatile unsigned long dly)
{
	for(; dly > 0; dly--);
}

void init_irq(void)
{
    int i=0;
    // while(1)
    // {
        // GPBDAT = 0xFFFFFF;
        // wait(30000);
        // GPBDAT = 0;
    // }
    // for(i=0;i<(sizeof(isr_handle_array)/sizeof(isr_handle_array[0]));i++)
    // {
        // isr_handle_array[i]     = Dummy_isr;
    // }
    
    // INTMOD = 0x0;         // �����ж϶���ΪIRQģʽ
    // INTMSK = BIT_ALLMSK;  // �����������ж�
    return ;
}
