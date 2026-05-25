# Makefile for libfs of FOOS
C_SOURCES=$(wildcard *.c)
OBJ=$(C_SOURCES:.c=.o)

.PHONY:	all clean dep clean-dep

all:	libfs.a

%.o:	%.c
	$(CC) -m32 -c -o $@ $< $(CFLAGS)

libfs.a:	$(OBJ)
	$(AR) rc $@ $^

clean:
	$(RM) -rf *.o *.a

install:
	mkdir -p $(DEST)/lib
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
