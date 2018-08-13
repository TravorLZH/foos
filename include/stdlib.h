#ifndef	STDLIB_H
#define	STDLIB_H
#ifndef	BUFSIZ
#define	BUFSIZ	512
#endif
#ifndef	NULL
#define	NULL	(void*)0
#endif
extern int atoi(const char *nptr);
extern void itoa(int n,char *s);
#endif
