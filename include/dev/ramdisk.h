#ifndef	RAMDISK_H
#define	RAMDISK_H
#include <inttypes.h>
#ifndef	FOOS_TOOLS
#include <foos/device.h>
#endif
#define	RD_HEADSIG	0xD158
#define	RD_FILESIG	0xDEADF11E

#define	RD_USEFILE	0x00
#define	RD_GETFILE	0x10

struct rd_header {
	uint16_t signature;	// Signature: 0xD158 (like DISK)
	uint16_t nfiles;
} __attribute__((packed));

struct rd_fileheader {
	uint32_t magic;		// Magic number: 0xDEADF11E (like Dead File)
	char name[32];		// File name
	uint32_t offset;	// Offset where file content starts
	uint32_t size;
} __attribute__((packed));

struct rd_data {
	char name[32];		// Requested file
	uint32_t flags;		// Unimplemented
	int fd;
} __attribute__((packed));

#ifndef	FOOS_TOOLS
extern int ramdisk_init(void *addr,uint32_t size);

/** RAM Disk device functions */
extern size_t ramdisk_read(struct device *dev,void *buf,size_t off,size_t len);
extern int ramdisk_open(struct device *dev,int flags);
extern int ramdisk_close(struct device *dev);
extern int ramdisk_ioctl(struct device *dev,int request,void *args);
#endif
#endif
