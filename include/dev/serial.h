#ifndef	SERIAL_H
#define	SERIAL_H

#include <inttypes.h>

/* Serial port addresses */
#define	SERIAL_COM1	0x3F8
#define	SERIAL_COM2	0x2F8
#define	SERIAL_COM3	0x3E8
#define	SERIAL_COM4	0x2E8

#define	SERIAL_PORT	SERIAL_COM1	/* Use COM1 as the default port */

/* When DLAB is disabled */
#define	SERIAL_DATA	(SERIAL_PORT)
#define	SERIAL_INT	(SERIAL_PORT+1)	/* Interrupt Enable Register */

/* When DLAB is enabled */
#define	SERIAL_LODIV	(SERIAL_PORT)	/* Baud rate divisor [3:0] */
#define	SERIAL_HIDIV	(SERIAL_PORT+1)	/* Baud rate divisor [7:4] */

#define	SERIAL_CHECKINT	(SERIAL_PORT+2)	/* Interrupt Identification */
#define	SERIAL_FIFO	(SERIAL_PORT+2)	/* FIFO control */

#define	SERIAL_LINE	(SERIAL_PORT+3)	/* Line control */
#define	SERIAL_LINESTAT	(SERIAL_PORT+5)	/* Line status */

#define	SERIAL_MODEM	(SERIAL_PORT+4)	/* Modem control */
#define	SERIAL_MODEMSTAT	(SERIAL_PORT+6)	/* Modem status */

#define	serial_disable_ints()	outb(SERIAL_INT,0)
#define	serial_enable_ints()	outb(SERIAL_INT,1)

extern void serial_init(void);
extern void serial_send(char c);
extern char serial_read(void);

extern int serial_write(const void *buf,size_t len);
extern int serial_print(const char *s);

extern int serial_printf(const char *fmt,...);

#endif
