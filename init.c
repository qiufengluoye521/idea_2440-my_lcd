#define     WTCON           (*(volatile unsigned long *)0x53000000)

/* SDRAM regisers */
#define     MEM_CTL_BASE    0x48000000
#define     SDRAM_BASE      0x30000000

/*clock registers*/
#define     CLKDIVN         (*(volatile unsigned long *)0x4c000014)
#define     MPLLCON         (*(volatile unsigned long *)0x4c000004)

#define     GSTATUS1        (*(volatile unsigned long *)0x560000B0)


#define S3C2410_MPLL_200MHZ     ((0x5c<<12)|(0x04<<4)|(0x00))
#define S3C2440_MPLL_200MHZ     ((0x5c<<12)|(0x01<<4)|(0x02))

#define S3C2440_MPLL_100MHZ     ((0x2A<<12)|(0x01<<4)|(0x02))
#define S3C2440_MPLL_50MHZ     ((0x11<<12)|(0x01<<4)|(0x02))


/*
 * 关闭WATCHDOG，否则CPU会不断重启
 */
void disable_watch_dog(void)
{
    WTCON = 0;  // 关闭WATCHDOG很简单，往这个寄存器写0即可
}

/*
 * 对于MPLLCON寄存器，[19:12]为MDIV，[9:4]为PDIV，[1:0]为SDIV
 * 有如下计算公式：
 *  S3C2410: MPLL(FCLK) = (m * Fin)/(p * 2^s)
 *  S3C2440: MPLL(FCLK) = (2 * m * Fin)/(p * 2^s)
 *  其中: m = MDIV + 8, p = PDIV + 2, s = SDIV
 * 对于本开发板，Fin = 12MHz
 * 设置CLKDIVN，令分频比为：FCLK:HCLK:PCLK=1:2:4，
 * FCLK=200MHz,HCLK=100MHz,PCLK=50MHz
 *
 * MPLL(FCLK)   = (2 * m * Fin)/(p * 2^s)
 *              = (2*(MDIV + 8)*12M)/((PDIV + 2)*2^SDIV)
 *              = (2*(42 + 8)*12M)/((1 + 2)*2^2)
 *              = 100*12/(3*4)
 *
 * FCLK=100MHz,HCLK=50MHz,PCLK=25MHz
 */
void clock_init(void)
{
    // LOCKTIME = 0x00ffffff;   // 使用默认值即可
    CLKDIVN  = 0x03;            // FCLK:HCLK:PCLK=1:2:4, HDIVN=1,PDIVN=1
    

    /* 如果HDIVN非0，CPU的总线模式应该从“fast bus mode”变为“asynchronous bus mode” */
__asm__(
    "mrc    p15, 0, r1, c1, c0, 0\n"        /* 读出控制寄存器 */ 
    "orr    r1, r1, #0xc0000000\n"          /* 设置为“asynchronous bus mode” */
    "mcr    p15, 0, r1, c1, c0, 0\n"        /* 写入控制寄存器 */
    );

    /* 判断是S3C2410还是S3C2440 */
    MPLLCON = S3C2440_MPLL_50MHZ;//376850;  /* 现在，FCLK=200MHz,HCLK=100MHz,PCLK=50MHz */
    // if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
    // {
        // MPLLCON = S3C2410_MPLL_200MHZ;  /* 现在，FCLK=200MHz,HCLK=100MHz,PCLK=50MHz */
    // }
    // else
    // {
        // MPLLCON = S3C2440_MPLL_200MHZ;//23570;  /* 现在，FCLK=200MHz,HCLK=100MHz,PCLK=50MHz */
    // }   
    return;
}

/*
 * 设置存储控制器以使用SDRAM
 */
void memsetup(void)
{
    volatile unsigned long *p = (volatile unsigned long *)MEM_CTL_BASE;

    /* 这个函数之所以这样赋值，而不是像前面的实验(比如mmu实验)那样将配置值
     * 写在数组中，是因为要生成”位置无关的代码”，使得这个函数可以在被复制到
     * SDRAM之前就可以在steppingstone中运行
     */
    /* 存储控制器13个寄存器的值 */
    p[0] = 0x22000000;     //BWSCON
    p[1] = 0x00000700;     //BANKCON0
    p[2] = 0x00000700;     //BANKCON1
    p[3] = 0x00000700;     //BANKCON2
    p[4] = 0x00000700;     //BANKCON3  
    p[5] = 0x00000700;     //BANKCON4
    p[6] = 0x00000700;     //BANKCON5
    p[7] = 0x00018005;     //BANKCON6
    p[8] = 0x00018005;     //BANKCON7
    
    /* REFRESH,
     * HCLK=12MHz:  0x008C07A3,
     * HCLK=100MHz: 0x008C04F4
     */ 
    p[9]  = 0x008C04F5;
    p[10] = 0x000000B1;     //BANKSIZE
    p[11] = 0x00000030;     //MRSRB6
    p[12] = 0x00000030;     //MRSRB7
}

void clean_bss(void)
{
    extern int __bss_start, __bss_end;
    int *p = &__bss_start;
    
    for (; p < &__bss_end; p++)
        *p = 0;
}

int CopyCode2SDRAM(unsigned char *buf, unsigned long start_addr, int size)
{    
    nand_read(buf, start_addr, size);
    return 0;
}