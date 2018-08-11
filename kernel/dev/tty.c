#include <dev/tty.h>
#include <cpu/interrupt.h>
#include <asm/ioports.h>
#include <inttypes.h>
#include <string.h>
#include <system.h>

static struct tty *current_tty=NULL;	// Caching the last-used TTY structure

static void kbd_irq(struct registers regs);

struct tty *tty_current(void)
{
	return current_tty;
}

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
	if(ptr==NULL){
		ptr=current_tty;
	}else{
		current_tty=ptr;
	}
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
	uint16_t x=VGA_X(current),y=VGA_Y(current),i;
	ptr+=current;
	switch(c){
	case '\n':
		y++;
	case '\r':
		current=VGA_COORD(0,y);
		break;
	case '\b':
		*--ptr=0x0720;
		current--;
		break;
	default:
		*ptr=(ptty->color << 8)+c;
		current++;
	}
	x=VGA_X(current);
	y=VGA_Y(current);
	if(y>=VGA_HEIGHT){
		y=VGA_HEIGHT-1;
		for(i=1;i<=y;i++){
			memcpy(VGA_BASE+VGA_COORD(0,i-1),
					VGA_BASE+VGA_COORD(0,i),
					VGA_WIDTH*2);
		}
		uint16_t *last=VGA_BASE+VGA_COORD(0,y);
		for(i=0;i<VGA_WIDTH;i++){
			last[i]=0x0720;
		}
	}
	tty_update_cursor(ptty,VGA_COORD(x,y));
}

size_t tty_writestring(struct tty *ptty,const char *string)
{
	char *str=(char*)string;
	do{
		tty_writechar(ptty,*str);
	}while(*++str);
	return str-string;
}

int tty_init(void *reserved)
{
	tty_enable_cursor(0xE,0xF);
	int_hook_handler(0x21,kbd_irq);
	return 0;
}

int tty_create(struct tty* ptr)
{
	current_tty=ptr;
	ptr->bufptr=ptr->buf;
	ptr->color=0x7;
	ptr->cursor=tty_get_cursor();
	ptr->flush=NULL;	// Let this be set by tty_read()
	return 0;
}
/***************************************************************************
 **************************** HERE COMES INPUT *****************************
 ***************************************************************************/


static const char scancode_set[]=
	"??1234567890-=\b\tqwertyuiop[]\n?asdfghjkl;'`?\\zxcvbnm,./??? ?";
static const char shift_set[]=
	"..!@#$%^&*()_+\b\tQWERTYUIOP{}\n.ASDFGHJKL:\"~.|ZXCVBNM<>?.*. ?";

static char toupper(char c)
{
	if(c>='a' && c<='z'){
		return (c=c+'A'-'a');
	}else{
		return c;
	}
}

static char tolower(char c)
{
	if(c>='A' && c<='Z'){
		return (c=c+'a'-'A');
	}else{
		return c;
	}
}

static char _decode(uint8_t code)
{
	struct tty* ptr=tty_current();
	if(!ptr->kbd.shift&&ptr->kbd.caps){
		// CAPS without SHIFT
		return toupper(scancode_set[code]);
	}else if(ptr->kbd.shift&&!ptr->kbd.caps){
		// SHIFT without CAPS
		return shift_set[code];
	}
	return scancode_set[code];
}

static void kbd_irq(struct registers regs)
{
	uint8_t scancode=inb(0x60);
	struct tty *ptr=tty_current();
	ptr->kbd.irq=1;	// IRQ is now fired!
	ptr->kbd.special=1;
	if(scancode & 0x80){
		scancode&=0x7F;
		switch(scancode){
		case CTRL:
			ptr->kbd.ctrl=0;
			break;
		case LSHIFT:case RSHIFT:
			ptr->kbd.shift=0;
			break;
		case ALT:
			ptr->kbd.alt=0;
			break;
		}
	}else{
		switch(scancode){
		case CTRL:
			ptr->kbd.ctrl=1;
			break;
		case LSHIFT:case RSHIFT:
			ptr->kbd.shift=1;
			break;
		case CAPS:
			ptr->kbd.caps^=1;
			break;
		case ALT:
			ptr->kbd.alt=1;
			break;
		case ENTER:
			ptr->kbd.special=0;
			tty_writechar(tty_current(),'\n');
			ptr->bufptr=ptr->buf;
			if(ptr->flush!=NULL){
				ptr->flush(ptr);
			}
			break;
		case BACKSPACE:
			ptr->kbd.special=0;
			tty_writechar(tty_current(),'\b');
			if(ptr->bufptr>ptr->buf){
				*--(ptr->bufptr)='\0';
			}
			break;
		default:
			ptr->kbd.special=0;
			char ch=_decode(scancode);
			tty_writechar(tty_current(),ch);
			*(ptr->bufptr++)=ch;
			*(ptr->bufptr)='\0';
		}
	}
}
