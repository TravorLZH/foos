# Makefile of FOOS kernel

AS=i386-elf-as
LD=i386-elf-ld

all:	floppy.img

boot/bootsect.bin:	boot/bootsect.s
	$(AS) -o $(<:.s=.o) $<
	$(LD) --oformat=binary -Ttext=0 -eboot -o $@ $(<:.s=.o)

floppy.img:	boot/bootsect.bin
	cp $< $@

run:
	qemu-system-i386 -fda floppy.img
