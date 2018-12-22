#ifndef	SYSTEM_H
#define	SYSTEM_H
#include <inttypes.h>
#define	CODESEG	0x08
#define	DATASEG	0x08
#define	NULL	(void*)0
#define	hang()	while(1)

#define	ARRAY_SIZE(x)	(sizeof((x)) / sizeof((x)[0]))

#ifndef	BUFSIZ
#define	BUFSIZ	512
#endif

#define	SEEK_SET	0
#define	SEEK_CUR	1
#define	SEEK_END	2

#define	KF_RAMDISK	0x01

struct kernel_conf {
	uint32_t flags;
	void* rd_start;
	void* rd_end;
} __attribute__((packed));

/* Functions */
extern int kernel_main(struct kernel_conf *conf);
#endif
