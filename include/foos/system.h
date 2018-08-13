#ifndef	SYSTEM_H
#define	SYSTEM_H
#include <dev/tty.h>
#define	CODESEG	0x08
#define	DATASEG	0x08
#define	NULL	(void*)0
#define	hang()	while(1)
#ifndef	BUFSIZ
#define	BUFSIZ	512
#endif
/* Common Variables */
extern struct tty *kernel_tty;
/* Functions */
extern int kernel_main(void *reserved);
#endif
