#ifndef	RAMDISK_H
#define	RAMDISK_H
#include <inttypes.h>
#include <foos/device.h>

/** Device commands for RAM Disk */
#define	RD_SETADDR	0x01
#define	RD_SETSIZE	0x02
#define	RD_GETADDR	0x11
#define	RD_GETSIZE	0x12

struct ramdisk {
	void *addr;
	size_t size;
};

/** RAM Disk device functions */
extern size_t ramdisk_read(struct device *dev,void *buf,size_t off,size_t len);
extern int ramdisk_open(struct device *dev,int flags);
extern int ramdisk_close(struct device *dev);
extern int ramdisk_ioctl(struct device *dev,int request,void *args);
#endif
