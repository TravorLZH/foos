#ifndef	STRING_H
#define	STRING_H
#include <inttypes.h>

static inline void *memcpy(void *dest,const void *src,size_t n)
{
	__asm__("cld\n"
		"rep movsb"::"D"(dest),"S"(src),"c"(n));
	return dest;
}

static inline void *memset(void *s,int c,size_t n)
{
	__asm__("cld\n"
		"rep stosb"::"D"(s),"a"((uint8_t)c),"c"(n));
	return s;
}
#endif
