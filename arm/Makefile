TARGET=arm-none-eabi
AS=$(TARGET)-as
LD=$(TARGET)-ld
OBJCOPY=$(TARGET)-objcopy
CC=$(TARGET)-gcc
CFLAGS=-ffreestanding
RM=rm
QEMU=qemu-system-arm

all:	kernel.img

kernel.img:	kernel.elf
	$(OBJCOPY) -Obinary $< $@

kernel.elf:	start.o main.o
	$(LD) -Tlink.ld -o$@ $^

start.o:	start.S
	$(AS) -o$@ $^

run:
	$(QEMU) -M versatilepb -kernel kernel.img -serial stdio

clean:
	$(RM) -rf *.img *.o
