#ifndef	FS_H
#define	FS_H
#include <inttypes.h>

#define	FS_FILE	0x01
#define	FS_DIR	0x02
#define	FS_CHAR	0x03
#define	FS_BLK	0x04
#define	FS_PIPE	0x05
#define	FS_SYM	0x06
#define	FS_MNT	0x08

struct inode;

typedef int (*openfunc)(struct inode*,uint32_t);
typedef int (*closefunc)(struct inode*);
typedef int (*writefunc)(struct inode*,const void*,size_t,off_t);
typedef int (*readfunc)(struct inode*,void*,size_t,off_t);

struct inode {
	char name[128];
	size_t size;
	uint32_t ino;
	uint8_t type;
	uint8_t flags;
	openfunc open;
	closefunc close;
	writefunc write;
	readfunc read;
};

struct dirent {
	char name[128];
	uint32_t ino;
};

extern int fs_open(struct inode*,int flags);
extern int fs_close(struct inode*);
extern int fs_write(struct inode*,const void*,size_t,off_t);
extern int fs_read(struct inode*,void*,size_t,off_t);
#endif
