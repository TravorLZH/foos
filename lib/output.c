#include <system.h>
#include <stdio.h>
#include <dev/tty.h>

int puts(const char *s)
{
	int ret=tty_writestring(&kernel_tty,s);
	tty_writechar(&kernel_tty,'\n');
}

int putchar(int c)
{
	tty_writechar(&kernel_tty,c);
	return c;
}
