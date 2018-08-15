C_SOURCES=$(filter-out dev/pit.c,$(wildcard *.c dev/*.c))
AS_SOURCES=$(filter-out entry.s,$(wildcard *.s dev/*.s))
OBJ=$(C_SOURCES:.c=.o) $(AS_SOURCES:.s=.o)
CPPFLAGS=-I../include
CFLAGS=-ffreestanding
CFLAGS+=-Wno-packed-bitfield-compat
CFLAGS+=$(CPPFLAGS)
LDFLAGS+=-melf_i386
.PHONY:	all clean dep clean-dep

all:	kernel.bin

%.o:	%.s
	$(AS) --32 -o $@ $<

%.o:	%.c
	$(CC) -m32 -c -o $@ $< $(CFLAGS)

kernel.bin:	entry.o $(OBJ) ../lib/libc.a ../liballoc/liballoc.a
	$(LD) $(LDFLAGS) -Tlink.ld -o $@ $^ --oformat=binary

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
