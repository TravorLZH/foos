# Makefile of FOOS kernel

AS=i386-elf-as
LD=i386-elf-ld
CC=i386-elf-gcc
RM=rm
QEMU=qemu-system-i386
LDFLAGS=--oformat=binary -Ttext=0

all:	floppy.img

%.bin:	%.s
	$(AS) -o $(<:.s=.o) $<
	$(LD) $(LDFLAGS) -o $@ $(<:.s=.o)

floppy.img:	boot/bootsect.bin boot/setup.bin kernel/kernel.bin
	cat $^ > $@

%.o:	%.c
	$(CC) -o $@ -c $< -ffreestanding

%.o:	%.s
	$(AS) -o $@ $<

kernel/kernel.bin:	kernel/entry.o kernel/main.o
	$(LD) $(LDFLAGS) -o $@ $^

run:
	$(QEMU) -fda floppy.img

clean:
	$(RM) -rf *.img *.iso
	$(RM) -rf boot/*.bin boot/*.o
