#include <foos/system.h>
#include <foos/fs.h>
#include <foos/devfs.h>
#include <foos/device.h>
#include <foos/kmalloc.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

#define	CHECK_DEV(d) \
	assert(d != NULL); \
	if(!((d)->flags & FS_CHAR) && !((d)->flags & FS_BLK)){ \
		errno=EINVAL; \
		return -EINVAL; \
	}

static struct dirent dirent;

int devfs_init(struct inode *devdir)
{
	int i;
	assert(devdir != NULL);
	strcpy(devdir->name,"dev");
	devdir->flags=FS_DIR;
	/* `impl' in this scope means how many files this directory has */
	devdir->impl=ndevs;
	devdir->ptr=kmalloc(ndevs * sizeof(struct inode));
	memset(devdir->ptr,0,ndevs * sizeof(struct inode));
	for(i=0;i<ndevs;i++){
		strcpy(devdir->ptr[i].name,devs[i].name);
		devdir->ptr[i].flags=FS_CHAR;	// Character device
		devdir->ptr[i].impl=i;		// `impl' = device number
		devdir->ptr[i].read=devfs_read;
		devdir->ptr[i].write=devfs_write;
	}
	return 0;
}

int devfs_write(struct inode *dev,const void* buf,size_t sz,off_t off)
{
	CHECK_DEV(dev);
	dev_seek(dev->impl,off,SEEK_SET);
	return dev_write(dev->impl,buf,sz);
}

int devfs_read(struct inode *dev,void *buf,size_t sz,off_t off)
{
	CHECK_DEV(dev);
	dev_seek(dev->impl,off,SEEK_SET);
	return dev_read(dev->impl,buf,sz);
}
