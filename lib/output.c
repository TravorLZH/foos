#include <system.h>
#include <stdio.h>
#include <dev/tty.h>

int _puts(const char *s)
{
	return tty_writestring(&kernel_tty,s);
}

int puts(const char *s)
{
	_puts(s);
	putchar('\n');
}

int putchar(int c)
{
	tty_writechar(&kernel_tty,c);
	return c;
}
