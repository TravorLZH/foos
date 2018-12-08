#ifndef	RAMFS_H
#define	RAMFS_H
#include <inttypes.h>

/** File system driver for FOOS RAM Drive */
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

#ifndef	FOOS_TOOLS
#include <foos/fs.h>

extern struct inode *ramfs_init(void);
extern struct dirent *ramfs_readdir(struct inode *node,size_t index);
#endif

#endif
