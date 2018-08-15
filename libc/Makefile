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
