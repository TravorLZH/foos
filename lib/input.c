#include <dev/tty.h>
#include <stdio.h>

int getchar(void)
{
	return tty_readchar(NULL);
}

char *gets(char *buf)
{
	struct tty *ptr=tty_current();
	tty_read(ptr,buf,BUFSIZ);
	return buf;
}
