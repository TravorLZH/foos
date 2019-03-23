#ifndef	IOPORTS_H
#define	IOPORTS_H
#include <inttypes.h>

/* For 8-bit ports */
#define	outb(port,val) \
	__asm__("outb %0,%1"::"a"((uint8_t)(val)),"Nd"((uint16_t)(port)));

#define	inb(port) ({ \
		uint8_t ret; \
		__asm__("inb %1,%0":"=a"(ret):"Nd"((uint16_t)(port))); \
		ret; \
		})

/* For 16-bit ports */
#define	outw(port,val) \
	__asm__("outw %0,%1"::"a"((uint16_t)(val)),"Nd"((uint16_t)(port)));

#define	inbw(port) ({ \
		uint16_t ret; \
		__asm__("inw %1,%0":"=a"(ret):"Nd"((uint16_t)(port))); \
		ret; \
		})

/* For 32-bit ports */
#define	outl(port,val) \
	__asm__("outl %0,%1"::"a"((uint32_t)(val)),"Nd"((uint16_t)(port)));

#define	inl(port) ({ \
		uint32_t ret; \
		__asm__("inl %1,%0":"=a"(ret):"Nd"((uint16_t)(port))); \
		ret; \
		})
#endif
