# Makefile of FOOS kernel

AS=i386-elf-as
LD=i386-elf-ld
CC=i386-elf-gcc
AR=i386-elf-ar
RM=rm
QEMU=qemu-system-i386
LDFLAGS=-melf_i386 --oformat=binary -Ttext=0
DEST=$(CURDIR)

.PHONY:	all all-subdirs install-libs run clean dep clean-dep ramdisk.img

all:	all-subdirs ramdisk.img floppy.img

all-subdirs:	all-libs install-libs
	$(MAKE) -C kernel CC=$(CC) LD=$(LD) AS=$(AS) AR=$(AR)
	$(MAKE) -C tools

all-libs:
	$(MAKE) -C libc CC=$(CC) LD=$(LD) AS=$(AS) AR=$(AR)
	$(MAKE) -C libfs CC=$(CC) LD=$(LD) AS=$(AS) AR=$(AR)
	$(MAKE) -C liballoc compile CC=$(CC) CFLAGS=-I../include AR=$(AR)

install-libs:
	$(MAKE) -C libc install DEST=$(DEST)
	$(MAKE) -C libfs install DEST=$(DEST)
	$(MAKE) -C liballoc install DEST=$(DEST)

%.bin:	%.s
	$(AS) --32 -o $(<:.s=.o) $<
	$(LD) $(LDFLAGS) -o $@ $(<:.s=.o)

ramdisk.img:
	tools/mkdisk ramdisk.img disk

floppy.img:	boot/bootsect.bin boot/setup.bin kernel/kernel.bin
	dd if=/dev/zero status=noxfer of=$@ count=2880
	cat $^ | dd status=noxfer conv=notrunc of=$@

run:
	$(QEMU) -d guest_errors -fda floppy.img -hda ramdisk.img

clean:
	$(RM) -rf *.img *.iso
	$(RM) -rf boot/*.bin boot/*.o
	$(MAKE) -C kernel $@ RM=$(RM)
	$(MAKE) -C libc $@ RM=$(RM)
	$(MAKE) -C libfs $@ RM=$(RM)
	$(MAKE) -C liballoc $@
	$(MAKE) -C tools $@


dep:
	$(MAKE) -C kernel $@ RM=$(RM)
	$(MAKE) -C libc $@ RM=$(RM)
	$(MAKE) -C libfs $@ RM=$(RM)

clean-dep:
	$(MAKE) -C kernel $@ RM=$(RM)
	$(MAKE) -C libc $@ RM=$(RM)
	$(MAKE) -C libfs $@ RM=$(RM)
