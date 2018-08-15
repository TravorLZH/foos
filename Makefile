# Makefile of FOOS kernel

AS=i386-elf-as
LD=i386-elf-ld
CC=i386-elf-gcc
AR=i386-elf-ar
RM=rm
QEMU=qemu-system-i386
LDFLAGS=-melf_i386 --oformat=binary -Ttext=0

.PHONY:	all all-subdirs run clean dep clean-dep

all:	all-subdirs floppy.img

all-subdirs:
	$(MAKE) -C lib CC=$(CC) LD=$(LD) AS=$(AS) AR=$(AR)
	$(MAKE) -C liballoc foos CC=$(CC) CFLAGS=-I../include AR=$(AR)
	$(MAKE) -C kernel CC=$(CC) LD=$(LD) AS=$(AS) AR=$(AR)

%.bin:	%.s
	$(AS) --32 -o $(<:.s=.o) $<
	$(LD) $(LDFLAGS) -o $@ $(<:.s=.o)

floppy.img:	boot/bootsect.bin boot/setup.bin kernel/kernel.bin
	dd if=/dev/zero status=noxfer of=$@ count=2880
	cat $^ | dd status=noxfer conv=notrunc of=$@

run:
	$(QEMU) -d guest_errors -fda floppy.img

clean:
	$(RM) -rf *.img *.iso
	$(RM) -rf boot/*.bin boot/*.o
	$(MAKE) -C kernel $@ RM=$(RM)
	$(MAKE) -C lib $@ RM=$(RM)

dep:
	$(MAKE) -C kernel $@ RM=$(RM)
	$(MAKE) -C lib $@ RM=$(RM)

clean-dep:
	$(MAKE) -C kernel $@ RM=$(RM)
	$(MAKE) -C lib $@ RM=$(RM)
