# Makefile of FOOS kernel

AS=i386-elf-as
LD=i386-elf-ld
RM=rm
QEMU=qemu-system-i386
boot_LDFLAGS=--oformat=binary -Ttext=0

all:	floppy.img

%.bin:	%.s
	$(AS) -o $(<:.s=.o) $<
	$(LD) $(boot_LDFLAGS) -o $@ $(<:.s=.o)

floppy.img:	boot/bootsect.bin boot/setup.bin boot/setup32.bin
	cat $^ > $@

run:
	$(QEMU) -fda floppy.img

clean:
	$(RM) -rf *.img *.iso
	$(RM) -rf boot/*.bin boot/*.o
