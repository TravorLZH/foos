#include <asm/ioports.h>
#include <foos/system.h>
#include <dev/serial.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

static char serial_enabled=0;

void serial_init(void)
{
	serial_disable_ints();
	outb(SERIAL_LINE,0x03);	// 8 (5+3) bits, 1 stop bit, no parity bit
	/* I do not understand what this does */
	outb(SERIAL_FIFO,0xC7);	// Enable FIFO
	serial_enabled=1;
	assert(serial_enabled != 0);
	serial_print("[serial] enabled!\n");
}

char serial_read(void)
{
	if(serial_enabled==0)
		return 0;
	while((inb(SERIAL_LINESTAT) & 0x1)==0);
	char c=inb(SERIAL_DATA);
	serial_send(c);
	return c;
}

int serial_write(const void *buf,size_t len)
{
	size_t i;
	const char *tmp=(const char*)buf;
	for(i=0;i<len;i++)
		serial_send(tmp[i]);
	return i;
}

int serial_print(const char *s)
{
	const char *original=s;
	if(*s=='\0' || s==NULL)
		return 0;
	do
		serial_send(*s);
	while(*(++s));
	return s-original;
}

int serial_printf(const char *fmt,...)
{
	char buf[BUFSIZ];
	va_list vlist;
	va_start(vlist,fmt);
	vsprintf(buf,fmt,vlist);
	va_end(vlist);
	return serial_print(buf);
}

void serial_send(char c)
{
	if(serial_enabled==0)
		return;
	if(c=='\n')
		serial_send('\r');
	while((inb(SERIAL_LINESTAT) & 0x20)==0);
	outb(SERIAL_DATA,c);
}
