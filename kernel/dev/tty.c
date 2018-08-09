#include <dev/tty.h>
#include <inttypes.h>
#include <asm/ioports.h>

static uint8_t color;

void tty_enable_cursor(uint8_t start,uint8_t end)
{
	outb(0x3D4,0xA);
	outb(0x3D5,(inb(0x3D5) & 0xC0) | start);
	outb(0x3D4,0xB);
	outb(0x3D5,(inb(0x3D5) & 0xE0) | end);
}

void tty_disable_cursor(void)
{
	outb(0x3D5,0xA);
	outb(0x3D5,0x20);
}

uint16_t tty_get_cursor(void)
{
	uint16_t ret;
	outb(0x3D4,0xE);
	ret=inb(0x3D5) << 8;
	outb(0x3D4,0xF);
	ret+=inb(0x3D5);
	return ret;
}

void tty_update_cursor(struct tty *ptr,uint16_t offset)
{
	outb(0x3D4,0xF);
	outb(0x3D5,offset & 0xFF);
	outb(0x3D4,0xE);
	outb(0x3D5,(offset >> 8) & 0xFF);
	ptr->cursor=offset;
}

size_t tty_write(struct tty *ptty,const void *ptr,size_t len)
{
	const char *data=(const char*)ptr;
	size_t i;
	for(i=0;i<len;i++){
		tty_writechar(ptty,data[i]);
	}
	return i;
}

void tty_writechar(struct tty *ptty,char c)
{
	uint16_t *ptr=VGA_BASE;
	uint16_t current=tty_get_cursor();
	uint16_t x=VGA_X(current),y=VGA_Y(current);
	ptr+=current;
	switch(c){
	case '\n':
		y++;
		tty_update_cursor(ptty,VGA_COORD(0,y));
		break;
	case '\r':
		tty_update_cursor(ptty,VGA_COORD(0,y));
		break;
	default:
		*ptr=(ptty->color << 8)+c;
		tty_update_cursor(ptty,++current);
	}
}

size_t tty_writestring(struct tty *ptty,const char *string)
{
	char *str=(char*)string;
	do{
		tty_writechar(ptty,*str);
	}while(*++str);
	return str-string;
}

int tty_init(struct tty* ptr)
{
	// TODO: Initialize IRQs and cursor
	tty_enable_cursor(0xE,0xF);
	ptr->color=0x7;
	ptr->cursor=tty_get_cursor();
	return 0;
}
