#ifndef	TARFS_H
#define	TARFS_H
#include <foos/fs.h>

/** Wiki: '0' or ASCII NUL */
#define	TARFS_FILE	'0'
#define	TARFS_FILE2	'\0'
/*
#define	TARFS_HARDLINK	'1'
#define	TARFS_SYMLINK	'2'
#define	TARFS_CHARDEV	'3'
#define	TARFS_BLKDEV	'4'
*/
#define	TARFS_DIR	'5'
/* #define	TARFS_FIFO	'6' */

#define	TARHEADER_SIZE	512

struct tar_header {
	char filename[100];
	char mode[8];
	char uid[8];
	char gid[8];
	char size[12];
	char mtime[12];
	char chksum[8];
	char type;
	char linkname[100];
	char ustar[6];
	char ustar_ver[2];
	char owner[32];
	char group[32];
	char padding[183];
	char data[];
} __attribute__((packed));

extern struct inode *tarfs_init(void *addr);

#endif
