C_SOURCES=$(wildcard *.c dev/*.c)
AS_SOURCES=$(filter-out entry.s,$(wildcard *.s dev/*.s))
OBJ=$(C_SOURCES:.c=.o) $(AS_SOURCES:.s=.o)
CPPFLAGS=-I../include
CFLAGS=-ffreestanding
CFLAGS+=-Wno-packed-bitfield-compat
CFLAGS+=$(CPPFLAGS)
LDFLAGS+=-melf_i386 --oformat=binary -L../lib
LIBS=-lc -lfs
.PHONY:	all clean dep clean-dep kernel.bin

all:	kernel.bin

%.o:	%.s
	$(AS) --32 -o $@ $<

%.o:	%.c
	$(CC) -m32 -c -o $@ $< $(CFLAGS)

kernel.bin:	entry.o $(OBJ)
	$(LD) $(LDFLAGS) -Tlink.ld -o $@ $^ $(LIBS)

clean:
	$(RM) -rf *.o *.bin
	$(RM) -rf dev/*.o

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > Makefile_temp
	(for i in *.c;do $(CPP) $(CPPFLAGS) -M $$i;done) >> Makefile_temp
	(for i in dev/*.c;do echo -n dev/;$(CPP) $(CPPFLAGS) -M $$i;done) >> Makefile_temp
	cp Makefile_temp Makefile
	$(RM) -f Makefile_temp

clean-dep:
	sed '/\#\#\# Dependencies/q' < Makefile > Makefile_temp
	cp Makefile_temp Makefile
	$(RM) -f Makefile_temp
### Dependencies
assert.o: assert.c ../include/assert.h
interrupt.o: interrupt.c ../include/foos/system.h ../include/inttypes.h \
 ../include/foos/kmalloc.h ../include/cpu/interrupt.h \
 ../include/dev/pic.h ../include/string.h ../include/stdlib.h \
 ../include/stdio.h ../include/stdarg.h
kmalloc.o: kmalloc.c ../include/foos/kmalloc.h ../include/inttypes.h \
 ../include/foos/system.h ../include/dev/serial.h ../include/assert.h \
 ../include/errno.h ../include/stdio.h ../include/stdarg.h
main.o: main.c ../include/foos/system.h ../include/inttypes.h \
 ../include/foos/kmalloc.h ../include/foos/device.h \
 ../include/foos/ramfs.h ../include/foos/fs.h ../include/dev/serial.h \
 ../include/dev/tty.h ../include/dev/pic.h ../include/dev/pit.h \
 ../include/dev/ramdisk.h ../include/cpu/interrupt.h \
 ../include/cpu/memory.h ../include/stdio.h ../include/stdarg.h \
 ../include/stdlib.h ../include/string.h
pmem.o: pmem.c ../include/cpu/memory.h ../include/inttypes.h \
 ../include/foos/system.h ../include/foos/kmalloc.h
shell.o: shell.c ../include/foos/fs.h ../include/inttypes.h \
 ../include/foos/kmalloc.h ../include/asm/cmos.h ../include/stdio.h \
 ../include/stdarg.h ../include/string.h ../include/foos/system.h \
 ../include/assert.h
vmem.o: vmem.c ../include/cpu/memory.h ../include/inttypes.h \
 ../include/cpu/interrupt.h ../include/foos/system.h \
 ../include/foos/kmalloc.h ../include/stdio.h ../include/stdarg.h \
 ../include/string.h
dev/device.o: dev/device.c ../include/foos/device.h ../include/foos/system.h \
 ../include/inttypes.h ../include/dev/tty.h ../include/dev/ramdisk.h \
 ../include/errno.h ../include/stdio.h ../include/stdarg.h
dev/floppy.o: dev/floppy.c ../include/dev/dma.h ../include/dev/floppy.h \
 ../include/inttypes.h ../include/asm/ioports.h \
 ../include/cpu/interrupt.h ../include/stdio.h ../include/stdarg.h
dev/pic.o: dev/pic.c ../include/asm/ioports.h ../include/inttypes.h \
 ../include/dev/serial.h ../include/dev/pic.h
dev/pit.o: dev/pit.c ../include/dev/pit.h ../include/inttypes.h \
 ../include/cpu/interrupt.h ../include/asm/ioports.h ../include/stdio.h \
 ../include/stdarg.h ../include/errno.h
dev/ramdisk.o: dev/ramdisk.c ../include/foos/kmalloc.h ../include/inttypes.h \
 ../include/dev/ramdisk.h ../include/foos/device.h \
 ../include/foos/system.h ../include/assert.h ../include/string.h \
 ../include/stdio.h ../include/stdarg.h ../include/errno.h
dev/serial.o: dev/serial.c ../include/asm/ioports.h ../include/inttypes.h \
 ../include/foos/system.h ../include/dev/serial.h ../include/stdarg.h
dev/tty.o: dev/tty.c ../include/dev/tty.h ../include/inttypes.h \
 ../include/foos/device.h ../include/foos/system.h \
 ../include/dev/serial.h ../include/foos/kmalloc.h \
 ../include/cpu/interrupt.h ../include/asm/ioports.h ../include/string.h \
 ../include/ctype.h ../include/errno.h
