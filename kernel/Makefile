C_SOURCES=$(wildcard *.c dev/*.c)
AS_SOURCES=$(filter-out entry.S,$(wildcard *.S dev/*.S))
OBJ=$(C_SOURCES:.c=.o) $(AS_SOURCES:.S=.o)
CFLAGS+=-Wno-packed-bitfield-compat
LDFLAGS+=-melf_i386 --oformat=binary -L../lib -x -M
LIBS=-lc -lfs
.PHONY:	all clean dep clean-dep kernel.bin

all:	kernel.bin foos.sym

%.o:	%.S
	$(AS) --32 -o $@ $<

%.o:	%.c
	$(CC) -m32 -c -o $@ $< $(CFLAGS)

kernel.bin:	entry.o $(OBJ)
	$(LD) $(LDFLAGS) -Tlink.ld -o $@ $^ $(LIBS) \
		| (grep -E '^\s+0x\S{8,16}\s+\S+$$' \
		| (sed -E 's/^\s+0x\S{0,8}(\S{8})\s+(\S+)$$/\1 \2/g' \
		> foos.map))

foos.sym:	kernel.bin
	../tools/symtable foos.map ../foos.sym

clean:
	$(RM) -rf foos.map ../foos.sym
	$(RM) -rf *.o *.bin
	$(RM) -rf dev/*.o

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > Makefile_temp
	(for i in *.c;do $(CPP) $(CPPFLAGS) -M $$i;done) >> Makefile_temp
	(for i in dev/*.c;do /bin/echo -n dev/;$(CPP) $(CPPFLAGS) -M $$i;done) \
		>> Makefile_temp	# Must use /bin/echo or bugs will occur
	cp Makefile_temp Makefile
	$(RM) -f Makefile_temp

clean-dep:
	sed '/\#\#\# Dependencies/q' < Makefile > Makefile_temp
	cp Makefile_temp Makefile
	$(RM) -f Makefile_temp
### Dependencies
