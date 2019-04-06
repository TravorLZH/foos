C_SOURCES=$(wildcard *.c)
OBJ=$(C_SOURCES:.c=.o)
CFLAGS=-ffreestanding
CPPFLAGS=-I../include
CFLAGS+=$(CPPFLAGS)

.PHONY:	all clean dep clean-dep

all:	libc.a

%.o:	%.s
	$(AS) -o $@ $<

%.o:	%.c
	$(CC) -m32 -c -o $@ $< $(CFLAGS)

libc.a:	$(OBJ)
	$(AR) rc $@ $^

clean:
	$(RM) -rf *.o *.a

install:
	mkdir -p $(DEST)/lib
	install -c libc.a $(DEST)/lib

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
conv.o: conv.c ../include/string.h ../include/inttypes.h \
 ../include/foos/system.h ../include/ctype.h ../include/stdlib.h
errno.o: errno.c ../include/errno.h
input.o: input.c ../include/dev/tty.h ../include/inttypes.h \
 ../include/foos/device.h ../include/foos/system.h ../include/stdio.h \
 ../include/stdarg.h
output.o: output.c ../include/stdio.h ../include/stdarg.h \
 ../include/inttypes.h ../include/string.h ../include/foos/system.h \
 ../include/foos/device.h
printf.o: printf.c ../include/stdarg.h ../include/string.h \
 ../include/inttypes.h ../include/foos/system.h ../include/stdlib.h \
 ../include/stdio.h
string.o: string.c ../include/string.h ../include/inttypes.h \
 ../include/foos/system.h
strtok.o: strtok.c ../include/string.h ../include/inttypes.h \
 ../include/foos/system.h
