#include <dev/tty.h>
#include <foos/system.h>
#include <foos/kmalloc.h>
#include <foos/device.h>
#include <cpu/interrupt.h>
#include <asm/ioports.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>

static struct tty *current_tty=NULL;	// Caching the last-used TTY structure
char init=0;

static void kbd_irq(struct registers regs);

struct tty *tty_current(void)
{
	return current_tty;
}

void tty_enable_cursor(uint8_t start,uint8_t end)
{
	outb(VGA_CMD,0xA);
	outb(VGA_DATA,(inb(VGA_DATA) & 0xC0) | start);
	outb(VGA_CMD,0xB);
	outb(VGA_DATA,(inb(VGA_DATA) & 0xE0) | end);
}

void tty_disable_cursor(void)
{
	outb(VGA_DATA,0xA);
	outb(VGA_DATA,0x20);
}

uint16_t tty_get_cursor(void)
{
	uint16_t ret;
	outb(VGA_CMD,0xE);
	ret=inb(VGA_DATA) << 8;
	outb(VGA_CMD,0xF);
	ret+=inb(VGA_DATA);
	return ret;
}

void tty_update_cursor(struct tty *ptr,uint16_t offset)
{
	if(ptr==NULL){
		ptr=current_tty;
	}else{
		current_tty=ptr;
	}
	outb(VGA_CMD,0xF);
	outb(VGA_DATA,offset & 0xFF);
	outb(VGA_CMD,0xE);
	outb(VGA_DATA,(offset >> 8) & 0xFF);
	ptr->cursor=offset;
}

size_t tty_write(struct tty *ptty,const char *data,size_t len)
{
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
	if(init){
		return -1;
	}
	init=1;
	int_hook_handler(0x21,kbd_irq);
	return 0;
}

int tty_create(struct tty* ptr)
{
	tty_enable_cursor(0xE,0xF);
	memset(ptr,0,sizeof(struct tty));
	current_tty=ptr;
	ptr->bufptr=ptr->buf;
	ptr->color=0x7;
	ptr->cursor=tty_get_cursor();
	return 0;
}

void tty_clear(struct tty *ptty)
{
	uint16_t *ptr=VGA_BASE;
	ptr+=VGA_WIDTH*VGA_HEIGHT;
	while((ptr--)>VGA_BASE){
		*ptr=0x0720;
	}
	tty_update_cursor(ptty,0);
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
			ptr->kbd.flush=1;	// Tell read() to finish
			ptr->tmp=ptr->bufptr;	// Save the buf pointer
			ptr->bufptr=ptr->buf;
			break;
		case BACKSPACE:
			ptr->kbd.special=0;
			if(ptr->bufptr<=ptr->buf){
				ptr->bufptr=ptr->buf;
				break;
			}
			tty_writechar(tty_current(),'\b');
			*--(ptr->bufptr)='\0';
			break;
		default:
			ptr->kbd.special=0;
			char ch=_decode(scancode);
			tty_writechar(tty_current(),ch);
			*(ptr->bufptr)=ch;
			ptr->bufptr++;
			*(ptr->bufptr)='\0';
		}
	}
}

char tty_readchar(struct tty *ptr)
{
	if(ptr==NULL){
		ptr=tty_current();
	}
	current_tty=ptr;
	while(!ptr->kbd.irq||ptr->kbd.special);
	ptr->kbd.irq=0;
	return _decode(inb(0x60));
}

size_t tty_read(struct tty *ptr,char *buf,size_t len)
{
	// Flush before anything
	if(ptr==NULL){
		ptr=current_tty;
	}else{
		current_tty=ptr;
	}
	ptr->bufptr=ptr->buf;
	while(!ptr->kbd.flush);	// Wait until the enter is pressed
	ptr->kbd.flush=0;
	size_t size=(ptr->tmp)-(ptr->buf);
	ptr->tmp=ptr->buf;
	memcpy(buf,ptr->buf,size);
	return size;
}

/* Wrapping write_dev */
size_t ttydev_write(struct device *dev,const void *buf,size_t len)
{
	struct tty *ptr=(struct tty*)dev->data;
	return tty_write(ptr,buf,len);
}

size_t ttydev_read(struct device *dev,void *buf,size_t len)
{
	struct tty *ptr=(struct tty*)dev->data;
	return tty_read(ptr,buf,len);
}

int ttydev_open(struct device *dev,int flags)
{
	struct tty *ptr=NULL;
	if(dev->data){
		errno=EBUSY;
		return -EBUSY;
	}
	tty_init(NULL);
	ptr=(struct tty*)kmalloc(sizeof(struct tty));
	tty_create(ptr);
	dev->data=ptr;
	return 0;
}

int ttydev_close(struct device *dev)
{
	kfree(dev->data);
	dev->data=NULL;
	return 0;
}

int ttydev_ioctl(struct device *dev,int request,void *args)
{
	struct tty *ptr=(struct tty*)dev->data;
	uint16_t *cursor=NULL;
	uint8_t *color=NULL;
	switch(request){
	case TTY_CLEAR:
		tty_clear(ptr);
		break;
	case TTY_SETCSR:
		tty_update_cursor(ptr,*(uint16_t*)args);
		break;
	case TTY_GETCSR:
		cursor=(uint16_t*)args;
		*cursor=tty_get_cursor();
		break;
	case TTY_SETCOL:
		ptr->color=*(uint8_t*)args;
		break;
	case TTY_GETCOL:
		color=(uint8_t*)args;
		*color=ptr->color;
		break;
	default:
		errno=EINVAL;
		return -EINVAL;
	}
	return 0;
}
