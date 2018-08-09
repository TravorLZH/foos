# Makefile of FOOS kernel

AS=i386-elf-as
LD=i386-elf-ld
CC=i386-elf-gcc
AR=i386-elf-ar
RM=rm
QEMU=qemu-system-i386
LDFLAGS=--oformat=binary -Ttext=0

.PHONY:	all all-subdirs run clean dep clean-dep

all:	all-subdirs floppy.img

all-subdirs:
	$(MAKE) -C lib CC=$(CC) LD=$(LD) AS=$(AS) AR=$(AR)
	$(MAKE) -C kernel CC=$(CC) LD=$(LD) AS=$(AS) AR=$(AR)

%.bin:	%.s
	$(AS) -o $(<:.s=.o) $<
	$(LD) $(LDFLAGS) -o $@ $(<:.s=.o)

floppy.img:	boot/bootsect.bin boot/setup.bin kernel/kernel.bin
	cat $^ > $@

kernel/kernel.bin:

run:
	$(QEMU) -fda floppy.img

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
