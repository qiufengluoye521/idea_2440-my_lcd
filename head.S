@******************************************************************************
@ File：led_on.S
@ 功能：LED点灯程序，点亮LED1
@******************************************************************************       
            
.text
.global _start
_start:     
    LDR     SP,=4096            @ 设置栈，以下跳到C函数，所以要提前设置栈
    BL      disable_watch_dog   @ 关闭WATCHDOG，否则CPU会不断重启
    BL      clock_init          @ 设置MPLL，改变FCLK、HCLK、PCLK
    BL      memsetup            @ 设置存储控制器以使用SDRAM
    BL      nand_init           @ 初始化NAND Flash
    
    ldr     r0,=0x30000000      @ 复制到SDRAM,SDRAM的起始地址是0x30000000
    mov     r1,#4096            @ 源地址
    mov     r2,#16*1024         @ 复制长度
    bl      CopyCode2SDRAM      @ 调用C函数CopyCode2SDRAM
    
    bl      clean_bss           @ 清除bss段，未初始化或初值为0的全局/静态变量保存在bss段
    
    msr     cpsr_c,#0xd2            @ 进入中断模式
    ldr     sp,=0x31000000          @ 设置中断模式栈指针
    
    msr     cpsr_c,#0xdf            @ 进入系统模式
    ldr     sp,=0x34000000          @ 设置系统模式指针
    
    ldr     lr,=ret_initirq         @ 设置返回地址
    ldr     pc,=init_irq            @ 调用中断初始化函数
    
ret_initirq:
    msr     cpsr_c, #0x5f           @ 设置I-bit=0，开IRQ中断
    
    ldr     lr,=halt_loop           @ 设置返回地址
    ldr     pc,=main                @ 跳转至main
    
halt_loop:
            b           halt_loop
            # BL      led_flicker
            # LDR     R0,=0x56000010      @ R0设为GPBCON寄存器。此寄存器
                                        # @ 用于选择端口B各引脚的功能：
                                        # @ 是输出、是输入、还是其他
            # MOV     R1,#0x00000400        
            # STR     R1,[R0]             @ 设置GPB5为输出口, 位[11:10]=0b01
            
            # LDR     R0,=0x56000014      @ R0设为GPBDAT寄存器。此寄存器
                                        # @ 用于读/写端口B各引脚的数据
            # MOV     R1,#0x00000000      @ 此值改为0x00000010,
                                        # @ 可让LED1熄灭
            # STR     R1,[R0]             @ GPB5输出0，LED1点亮
MAIN_LOOP:
            B       MAIN_LOOP
            
led_flicker:
			// 初始化LED
			ldr r0, =0x56000010  @ GPBCON
			ldr r1, =(1<<(5*2))
			str r1, [r0]
			
			// 循环点亮熄灭LED
			ldr r0, =0x56000014  @ GPBDAT
			ldr r1, =0
			ldr r2, =(1<<5)
loop:			
			str r1, [r0]   @ 点亮
			bl delay
			str r2, [r0]   @ 熄灭
			bl delay
			b loop

delay:
			ldr r3, =300000
1:
			sub r3, r3, #1
			cmp r3, #0
			bne 1b
			mov pc, lr
