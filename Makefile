# Makefile of FOOS kernel

AS=i386-elf-as
LD=i386-elf-ld
CC=i386-elf-gcc
AR=i386-elf-ar
RM=rm
QEMU=qemu-system-i386
QEMUFLAGS=-d guest_errors -m 16M
LDFLAGS=-melf_i386 --oformat=binary -Ttext=0
DEST=$(CURDIR)

.PHONY:	all all-subdirs install-libs clean dep clean-dep ramdisk.img \
	ramdisk2.img
.IGNORE: run

all:	all-subdirs ramdisk.img ramdisk2.img bootdisk.img

all-subdirs:	all-libs install-libs
	$(MAKE) -C tools
	$(MAKE) -C kernel CC=$(CC) LD=$(LD) AS=$(AS) AR=$(AR)

all-libs:
	$(MAKE) -C libc CC=$(CC) LD=$(LD) AS=$(AS) AR=$(AR)
	$(MAKE) -C libfs CC=$(CC) LD=$(LD) AS=$(AS) AR=$(AR)

install-libs:
	$(MAKE) -C libc install DEST=$(DEST)
	$(MAKE) -C libfs install DEST=$(DEST)

%.bin:	%.s
	$(AS) --32 -o $(<:.s=.o) $<
	$(LD) $(LDFLAGS) -o $@ $(<:.s=.o)

ramdisk.img:
	tools/mkdisk $@ disk

ramdisk2.img:
	tar cvf $@ disk

bootdisk.img:	boot/bootsect.bin boot/setup.bin kernel/kernel.bin
	dd if=/dev/zero of=$@ count=1024
	cat $^ | dd conv=notrunc of=$@

run:
	$(QEMU) $(QEMUFLAGS) -hda bootdisk.img -hdb ramdisk.img -serial stdio

run-term:
	$(QEMU) $(QEMUFLAGS) -hda bootdisk.img -hdb ramdisk.img -curses

clean:
	$(RM) -rf lib/
	$(RM) -rf *.img *.iso
	$(RM) -rf boot/*.bin boot/*.o
	$(MAKE) -C kernel $@ RM=$(RM)
	$(MAKE) -C libc $@ RM=$(RM)
	$(MAKE) -C libfs $@ RM=$(RM)
	$(MAKE) -C tools $@

dep:
	$(MAKE) -C kernel $@ RM=$(RM)
	$(MAKE) -C libc $@ RM=$(RM)
	$(MAKE) -C libfs $@ RM=$(RM)

clean-dep:
	$(MAKE) -C kernel $@ RM=$(RM)
	$(MAKE) -C libc $@ RM=$(RM)
	$(MAKE) -C libfs $@ RM=$(RM)
