# Makefile for libfs of FOOS
C_SOURCES=$(wildcard *.c)
OBJ=$(C_SOURCES:.c=.o)
CFLAGS=-ffreestanding
CPPFLAGS=-I../include
CFLAGS+=$(CPPFLAGS)

.PHONY:	all clean dep clean-dep

all:	libfs.a

%.o:	%.c
	$(CC) -m32 -c -o $@ $< $(CFLAGS)

libfs.a:	$(OBJ)
	$(AR) rc $@ $^

clean:
	$(RM) -rf *.o *.a

install:
	install -c libfs.a $(DEST)/lib

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > Makefile_temp
	(for i in *.c;do $(CPP) $(CPPFLAGS) -M $$i;done) >> Makefile_temp
	cp Makefile_temp Makefile
	$(RM) -f Makefile_temp

clean-dep:
	sed '/\#\#\# Dependencies/q' < Makefile > Makefile_temp
	cp Makefile_temp Makefile
	$(RM) -f Makefile_temp
### Dependencies
ramfs.o: ramfs.c ../include/foos/fs.h ../include/inttypes.h \
 ../include/foos/device.h ../include/foos/system.h \
 ../include/foos/ramfs.h ../include/dev/ramdisk.h ../include/assert.h \
 ../include/stdio.h ../include/stdarg.h
wrap.o: wrap.c ../include/foos/system.h ../include/inttypes.h \
 ../include/foos/fs.h ../include/foos/device.h ../include/errno.h
