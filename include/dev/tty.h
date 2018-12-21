#ifndef	TTY_H
#define	TTY_H
#include <inttypes.h>
#include <foos/device.h>

/* Device Commands */
#define	TTY_CLEAR	0x00	/* Clear terminal screen */
#define	TTY_SETCSR	0x01	/* Set cursor position */
#define	TTY_GETCSR	0x81	/* Get cursor position */
#define	TTY_SETCOL	0x02	/* Set color */
#define	TTY_GETCOL	0x82	/* Get color */

#define	VGA_CMD		0x3D4
#define	VGA_DATA	(VGA_CMD+1)
#define	VGA_BASE	(uint16_t*)0xB8000
#define	VGA_WIDTH	80
#define	VGA_HEIGHT	25
#define	VGA_COORD(x,y)	((y)*VGA_WIDTH+(x))
#define	VGA_X(offset)	((offset)%VGA_WIDTH)
#define	VGA_Y(offset)	((offset)/VGA_WIDTH)
/* Keyboard */
#define	MAX_SCANCODE	58
#define	RELEASED(code)	((code) & 0x80)
#define	TAB	0x0F	// TAB cannot be used by keyboard now
#define	CTRL	0x1D
#define	LSHIFT	0x2A
#define	RSHIFT	0x36
#define	ENTER	0x1C
#define	ALT	0x38
#define	CAPS	0x3A
#define	BACKSPACE	0x0E

struct kbd_state{
	uint8_t ctrl:1;
	uint8_t shift:1;
	uint8_t alt:1;
	uint8_t caps:1;
	uint8_t irq:1;		// Used by TTY internal calls
	uint8_t special:1;	// Used by TTY internal calls as well
	uint8_t flush:1;	// Is the buffer flushed
	uint8_t reserved:1;
} __attribute__((packed));

struct tty{
	uint8_t color;
	uint8_t cursor;
	char last;	// Last printed character
	char buf[512];
	char *bufptr;
	char *tmp;
	struct kbd_state kbd;
} __attribute__((packed));

extern struct tty *tty_current(void);
extern void tty_enable_cursor(uint8_t start,uint8_t end);
extern void tty_disable_cursor(void);
extern uint16_t tty_get_cursor(void);
extern void tty_update_cursor(struct tty *ptr,uint16_t offset);
extern void tty_writechar(struct tty *ptr,char c);
extern size_t tty_write(struct tty *ptr,const char *data,size_t len);
extern size_t tty_writestring(struct tty *ptr,const char *str);
extern size_t tty_read(struct tty *ptr,char *buf,size_t len);
extern char tty_readchar(struct tty *ptr);
extern int tty_create(struct tty *ptr);
extern void tty_clear(struct tty *ptr);
/* Initialization before creating TTY */
extern int tty_init(void *reserved);

/* TTY device functions */
extern size_t ttydev_write(struct device *dev,const void *buf,size_t len);
extern size_t ttydev_read(struct device *dev,void *buf,size_t off,size_t len);
extern int ttydev_open(struct device *dev,int flags);
extern int ttydev_close(struct device *dev);
extern int ttydev_ioctl(struct device *dev,int request,void *args);
#endif
