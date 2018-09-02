#ifndef	RAMDISK_H
#define	RAMDISK_H
#include <inttypes.h>
#define	RD_HEADSIG	0xD158
#define	RD_FILESIG	0xDEADF11E

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

extern void initrd_init(void *addr);
#endif
