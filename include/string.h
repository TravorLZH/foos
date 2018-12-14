#ifndef	STRING_H
#define	STRING_H
#include <inttypes.h>

extern char *strrev(char *str);
extern char *strcpy(char *to,const char *from);
extern int strcmp(const char *s1,const char *s2);

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

static inline int memcmp(const void *a,const void *b,size_t n)
{
	register int ret __asm__("eax");
	__asm__("cld\n"
		"repe cmpsb\n"
		"je 1f\n"
		"movl $1,%%eax\n"
		"jl 1f\n"
		"negl %%eax\n"
		"1:"
		:"=a"(ret):"0"(0),"D"(a),"S"(b),"c"(n));
	return ret;
}
#endif
