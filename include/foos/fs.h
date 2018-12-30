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
struct dirent;

typedef int (*open_t)(struct inode*,uint32_t);
typedef int (*close_t)(struct inode*);
typedef int (*write_t)(struct inode*,const void*,size_t,off_t);
typedef int (*read_t)(struct inode*,void*,size_t,off_t);
typedef struct dirent *(*readdir_t)(struct inode*,size_t);
typedef struct inode *(*finddir_t)(struct inode*,const char*);

struct inode {
	char name[32];
	uint8_t flags;
	uint8_t mask;
	uint8_t uid;
	uint8_t gid;
	uint32_t size;
	uint32_t ino;
	uint32_t impl;		// Used by implementation
	open_t open;
	close_t close;
	write_t write;
	read_t read;
	readdir_t readdir;
	finddir_t finddir;
	struct inode *ptr;	// Mountpoints and Symbolic links
};

struct dirent {
	char name[32];
	uint32_t ino;
};

/* Global Variables */
extern struct inode *fs_root;

extern int fs_open(struct inode*,int flags);
extern int fs_close(struct inode*);
extern int fs_write(struct inode*,const void*,size_t,off_t);
extern int fs_read(struct inode*,void*,size_t,off_t);
extern struct dirent *fs_readdir(struct inode*,size_t);
extern struct inode *fs_finddir(struct inode*,const char*);
extern struct inode *fs_lookup(const char *path);
#endif
