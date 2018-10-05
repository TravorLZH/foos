TARGET=arm-none-eabi
AS=$(TARGET)-as
LD=$(TARGET)-ld
OBJCOPY=$(TARGET)-objcopy
CC=$(TARGET)-gcc
CFLAGS=-ffreestanding
RM=rm
QEMU=qemu-system-arm
QEMUFLAGS=-M raspi2 -nographic

all:	kernel.img

kernel.img:	kernel.elf
	$(OBJCOPY) -Obinary $< $@

kernel.elf:	start.o main.o
	$(LD) -Tlink.ld -o$@ $^

start.o:	start.S
	$(AS) -o$@ $^

run:
	$(QEMU) $(QEMUFLAGS) -kernel kernel.img

clean:
	$(RM) -rf *.img *.o
