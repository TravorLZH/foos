#ifndef	TTY_H
#define	TTY_H
#include <inttypes.h>
#define	VGA_BASE	(uint16_t*)0xB8000
#define	VGA_WIDTH	80
#define	VGA_HEIGHT	25
#define	VGA_COORD(x,y)	((y)*VGA_WIDTH+(x))
#define	VGA_X(offset)	((offset)%VGA_WIDTH)
#define	VGA_Y(offset)	((offset)/VGA_WIDTH)

struct tty{
	uint8_t color;
	uint8_t cursor;
	char buf[512];
};

extern void tty_enable_cursor(uint8_t start,uint8_t end);
extern void tty_disable_cursor(void);
extern uint16_t tty_get_cursor(void);
extern void tty_update_cursor(struct tty *ptr,uint16_t offset);
extern void tty_writechar(struct tty *ptr,char c);
extern size_t tty_write(struct tty *ptr,const void *data,size_t len);
extern size_t tty_writestring(struct tty *ptr,const char *str);
extern size_t tty_read(struct tty *ptr,char *buf,int size);
extern int tty_init(struct tty *ptr);
#endif
