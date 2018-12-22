#ifndef	DEVFS_H
#define	DEVFS_H

#include <foos/fs.h>

extern int devfs_init(struct inode*);
extern struct dirent *devfs_readdir(struct inode*,size_t);
extern int devfs_read(struct inode*,void*,size_t,off_t);
extern int devfs_write(struct inode*,const void*,size_t,off_t);

#endif
