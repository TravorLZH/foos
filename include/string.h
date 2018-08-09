#ifndef	STRING_H
#define	STRING_H
#include <inttypes.h>

extern char *strrev(char *str);

static inline int strlen(const char *s)
{
	register int ret __asm__("ecx");
	__asm__("cld\n"
		"repne scasb\n"
		"notl %0\n"
		"decl %0":"=c"(ret):"D"(s),"a"(0),"0"(0xFFFFFFFF));
	return ret;
}

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
