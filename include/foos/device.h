#ifndef	DEVICE_H
#define	DEVICE_H
#include <foos/system.h>
#include <inttypes.h>
/* Device no. */
#define	DEV_TTY		0
#define	DEV_RAMDISK	1

struct device;

typedef	size_t(*devwrite_t)(struct device*,const void*,size_t);
typedef size_t(*devread_t)(struct device*,void*,size_t,size_t);
typedef int(*devopen_t)(struct device*,int);
typedef int(*devclose_t)(struct device*);
typedef int(*devioctl_t)(struct device*,int,void*);

struct device {
	char name[16];
	size_t offset;	// Offset of buffer
	void *data;
	devwrite_t write;
	devread_t read;
	devopen_t open;
	devclose_t close;
	devioctl_t ioctl;
};

extern size_t dev_write(int no,const void *buf,size_t sz);
extern size_t dev_read(int no,void *buf,size_t sz);
extern int dev_open(int no,int flags);
extern int dev_close(int no);
extern int dev_ioctl(int no,int request,void *arg);
extern size_t dev_seek(int no,size_t off,int whence);
#endif
