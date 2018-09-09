@******************************************************************************
@ File��led_on.S
@ ���ܣ�LED��Ƴ��򣬵���LED1
@******************************************************************************       
            
.text
.global _start
_start:     
    LDR     SP,=4096            @ ����ջ����������C����������Ҫ��ǰ����ջ
    BL      disable_watch_dog   @ �ر�WATCHDOG������CPU�᲻������
    BL      clock_init          @ ����MPLL���ı�FCLK��HCLK��PCLK
    BL      memsetup            @ ���ô洢��������ʹ��SDRAM
    BL      nand_init           @ ��ʼ��NAND Flash
    
    ldr     r0,=0x30000000      @ ���Ƶ�SDRAM,SDRAM����ʼ��ַ��0x30000000
    mov     r1,#4096            @ Դ��ַ
    mov     r2,#16*1024         @ ���Ƴ���
    bl      CopyCode2SDRAM      @ ����C����CopyCode2SDRAM
    
    bl      clean_bss           @ ���bss�Σ�δ��ʼ�����ֵΪ0��ȫ��/��̬����������bss��
    
    msr     cpsr_c,#0xd2            @ �����ж�ģʽ
    ldr     sp,=0x31000000          @ �����ж�ģʽջָ��
    
    msr     cpsr_c,#0xdf            @ ����ϵͳģʽ
    ldr     sp,=0x34000000          @ ����ϵͳģʽָ��
    
    ldr     lr,=ret_initirq         @ ���÷��ص�ַ
    BL      led_flicker
    ldr     pc,=init_irq            @ �����жϳ�ʼ������
    
ret_initirq:
    BL      led_flicker
    msr     cpsr_c, #0x5f           @ ����I-bit=0����IRQ�ж�
    
    ldr     lr,=halt_loop           @ ���÷��ص�ַ
    # BL      led_flicker
    ldr     pc,=main                @ ��ת��main
    
halt_loop:
            b           halt_loop
            # BL      led_flicker
            # LDR     R0,=0x56000010      @ R0��ΪGPBCON�Ĵ������˼Ĵ���
                                        # @ ����ѡ��˿�B�����ŵĹ��ܣ�
                                        # @ ������������롢��������
            # MOV     R1,#0x00000400        
            # STR     R1,[R0]             @ ����GPB5Ϊ�����, λ[11:10]=0b01
            
            # LDR     R0,=0x56000014      @ R0��ΪGPBDAT�Ĵ������˼Ĵ���
                                        # @ ���ڶ�/д�˿�B�����ŵ�����
            # MOV     R1,#0x00000000      @ ��ֵ��Ϊ0x00000010,
                                        # @ ����LED1Ϩ��
            # STR     R1,[R0]             @ GPB5���0��LED1����
MAIN_LOOP:
            B       MAIN_LOOP
            
led_flicker:
			// ��ʼ��LED
			ldr r0, =0x56000010  @ GPBCON
			ldr r1, =(1<<(5*2))
			str r1, [r0]
			
			// ѭ������Ϩ��LED
			ldr r0, =0x56000014  @ GPBDAT
			ldr r1, =0
			ldr r2, =(1<<5)
loop:			
			str r1, [r0]   @ ����
			bl delay
			str r2, [r0]   @ Ϩ��
			bl delay
			b loop

delay:
			ldr r3, =300000
1:
			sub r3, r3, #1
			cmp r3, #0
			bne 1b
			mov pc, lr