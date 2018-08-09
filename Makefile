# Makefile of FOOS kernel

AS=i386-elf-as
LD=i386-elf-ld
CC=i386-elf-gcc
RM=rm
QEMU=qemu-system-i386
LDFLAGS=--oformat=binary -Ttext=0

.PHONY:	all run clean dep clean-dep kernel/kernel.bin

all:	floppy.img

%.bin:	%.s
	$(AS) -o $(<:.s=.o) $<
	$(LD) $(LDFLAGS) -o $@ $(<:.s=.o)

floppy.img:	boot/bootsect.bin boot/setup.bin kernel/kernel.bin
	cat $^ > $@

kernel/kernel.bin:
	$(MAKE) -C kernel kernel.bin CC=$(CC) LD=$(LD) AS=$(AS)

run:
	$(QEMU) -fda floppy.img

clean:
	$(RM) -rf *.img *.iso
	$(RM) -rf boot/*.bin boot/*.o
	$(MAKE) -C kernel $@

dep:
	$(MAKE) -C kernel $@

clean-dep:
	$(MAKE) -C kernel $@
