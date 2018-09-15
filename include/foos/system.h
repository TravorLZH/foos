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

#define	KF_RAMDISK	0x01

struct kernel_conf {
	uint32_t flags;
	void* rd_start;
	void* rd_end;
} __attribute__((packed));

/* Functions */
extern int kernel_main(struct kernel_conf *conf);
#endif
