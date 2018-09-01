#include <stdio.h>
#include <string.h>
#include <foos/device.h>

int _puts(const char *s)
{
	return dev_write(DEV_TTY,s,strlen(s));
}

int puts(const char *s)
{
	_puts(s);
	putchar('\n');
}

int putchar(int c)
{
	dev_write(DEV_TTY,&c,1);
	return c;
}
