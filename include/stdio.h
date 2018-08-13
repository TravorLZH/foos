#ifndef	STDIO_H
#define	STDIO_H
#include <stdarg.h>
#include <inttypes.h>
#ifndef	BUFSIZ
#define	BUFSIZ	512
#endif
#ifndef	NULL
#define	NULL	(void*)0
#endif
extern int putchar(int c);
extern int puts(const char *str);
extern int getchar(void);
extern char *gets(char *buf);

extern int printf(const char *fmt,...);
extern int sprintf(char *str,const char *fmt,...);
extern int vprintf(const char *fmt,va_list ap);
extern int vsprintf(char *str,const char *fmt,va_list ap);
#endif
