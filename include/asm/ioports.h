#ifndef	IOPORTS_H
#define	IOPORTS_H
#include <inttypes.h>

#define	outb(port,val) \
	__asm__("outb %0,%1"::"a"((uint8_t)(val)),"Nd"((uint16_t)(port)));

#define	inb(port) ({ \
		uint8_t ret; \
		__asm__("inb %1,%0":"=a"(ret):"Nd"((uint16_t)(port))); \
		ret; \
		})
#endif
