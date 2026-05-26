# Makefile of FOOS kernel

AS=i686-elf-as
LD=i686-elf-ld
CC=i686-elf-gcc
CFLAGS=-std=c99 -ffreestanding
CPP=i686-elf-cpp
CPPFLAGS=-I$(CURDIR)/include
CFLAGS+=$(CPPFLAGS)
AR=i686-elf-ar
RM=rm
QEMU=qemu-system-i386
QEMUFLAGS=-d guest_errors -m 16M
LDFLAGS=-melf_i386 --oformat=binary -Ttext=0
DEST=$(CURDIR)

.PHONY:	all tools all-subdirs install-libs clean dep clean-dep ramdisk.img \
	ramdisk2.img
.IGNORE: run

all:	tools all-subdirs ramdisk.img ramdisk2.img bootdisk.img

tools:
	$(MAKE) -C tools

all-subdirs:	all-libs install-libs kernel/kernel.bin

kernel/kernel.bin:
	$(MAKE) -C kernel CC=$(CC) LD=$(LD) AS=$(AS) AR=$(AR) CFLAGS="$(CFLAGS)" CPPFLAGS="$(CPPFLAGS)"

all-libs:
	$(MAKE) -C libc CC=$(CC) LD=$(LD) AS=$(AS) AR=$(AR) CFLAGS="$(CFLAGS)" CPPFLAGS="$(CPPFLAGS)"
	$(MAKE) -C libfs CC=$(CC) LD=$(LD) AS=$(AS) AR=$(AR) CFLAGS="$(CFLAGS)" CPPFLAGS="$(CPPFLAGS)"

install-libs:
	$(MAKE) -C libc install DEST=$(DEST)
	$(MAKE) -C libfs install DEST=$(DEST)

boot/sysvars.inc:	kernel/kernel.bin
	@syssize=$$(wc -c < $< | tr -d ' '); \
	printf "SYSSIZE=0x%x\n" $$syssize > $@;

%.bin:	%.s boot/sysvars.inc
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
	$(RM) -f boot/sysvars.inc
	$(MAKE) -C kernel $@ RM=$(RM)
	$(MAKE) -C libc $@ RM=$(RM)
	$(MAKE) -C libfs $@ RM=$(RM)
	$(MAKE) -C tools $@

dep:
	$(MAKE) -C kernel $@ RM=$(RM) CPP=$(CPP) CPPFLAGS="$(CPPFLAGS)"
	$(MAKE) -C libc $@ RM=$(RM) CPP=$(CPP) CPPFLAGS="$(CPPFLAGS)"
	$(MAKE) -C libfs $@ RM=$(RM) CPP=$(CPP) CPPFLAGS="$(CPPFLAGS)"

clean-dep:
	$(MAKE) -C kernel $@ RM=$(RM)
	$(MAKE) -C libc $@ RM=$(RM)
	$(MAKE) -C libfs $@ RM=$(RM)
