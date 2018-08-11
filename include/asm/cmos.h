#ifndef	CMOS_H
#define	CMOS_H
#include <inttypes.h>
static inline uint8_t cmos_read(uint8_t reg)
{
	register uint8_t ret __asm__("al");
	__asm__("outb %%al,$0x70"::"a"(reg));
	__asm__("inb $0x71,%%al":"=a"(ret));
	return ret;
}
#endif
