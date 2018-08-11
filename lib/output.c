#include <stdio.h>
#include <dev/tty.h>

int _puts(const char *s)
{
	return tty_writestring(tty_current(),s);
}

int puts(const char *s)
{
	_puts(s);
	putchar('\n');
}

int putchar(int c)
{
	tty_writechar(tty_current(),c);
	return c;
}
