
CC      = arm-linux-gcc
LD      = arm-linux-ld
AR      = arm-linux-ar
OBJCOPY = arm-linux-objcopy
OBJDUMP = arm-linux-objdump

INCLUDEDIR 	:= $(shell pwd)/include
CFLAGS 		:= -Wall -O2
CPPFLAGS   	:= -nostdinc -I$(INCLUDEDIR)

export 	CC LD AR OBJCOPY OBJDUMP INCLUDEDIR CFLAGS CPPFLAGS

objs := head.o serial.o nand.o interrupt.o lcddrv.o framebuffer.o lcdlib.o init.o main.o sunflower.o
lcd.bin: $(objs)
	${LD} -Tlcd.lds -o lcd_elf $^
	${OBJCOPY} -O binary -S lcd_elf $@
	${OBJDUMP} -D -m arm lcd_elf > lcd.dis


	
%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	rm -f lcd.bin lcd_elf lcd.dis *.o
	
