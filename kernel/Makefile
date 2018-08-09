C_SOURCES=$(wildcard *.c)
AS_SOURCES=$(filter-out entry.s,$(wildcard *.o))
OBJ=$(C_SOURCES:.c=.o) $(AS_SOURCES:.s=.o)

all:	kernel.bin

%.o:	%.s
	$(AS) -o $@ $<

%.o:	%.c
	$(CC) -c -o $@ $< -ffreestanding

kernel.bin:	entry.o $(OBJ)
	$(LD) -Tlink.ld -o $@ $^ --oformat=binary

clean:
	$(RM) -rf *.o *.bin
