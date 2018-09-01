#ifndef	DEVICE_H
#define	DEVICE_H
#include <inttypes.h>

#define	DEV_TTY	0

struct device;

typedef	size_t(*devwrite_t)(struct device*,const void*,size_t);
typedef size_t(*devread_t)(struct device*,void*,size_t);
typedef int(*devopen_t)(struct device*,uint8_t);
typedef int(*devclose_t)(struct device*);

struct device {
	char name[16];
	devwrite_t write;
	devread_t read;
	devopen_t open;
	devclose_t close;
	void *data;
};

extern size_t dev_write(int no,const void *buf,size_t sz);
extern size_t dev_read(int no,void *buf,size_t sz);
extern int dev_open(int no,uint8_t flags);
extern int dev_close(int no);
#endif
