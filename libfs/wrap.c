#include <foos/system.h>
#include <foos/fs.h>
#include <foos/device.h>
#include <errno.h>

#define	FIX_DEVICE(x,f,...) \
	if(x->flags==FS_CHAR||x->flags==FS_BLK){ \
		return dev_##f(x->ino,__VA_ARGS__); \
	}

int fs_open(struct inode *node,int flags)
{
	FIX_DEVICE(node,open,flags)
	if(node->open)
		return node->open(node,flags);
	else
		return -ENOSYS;
}

int fs_close(struct inode *node)
{
	if(node->flags==FS_CHAR||node->flags==FS_BLK){
		return dev_close(node->ino);
	}
	if(node->close)
		return node->close(node);
	else
		return -ENOSYS;
}

int fs_write(struct inode *node,const void *buf,size_t sz,off_t off)
{
	FIX_DEVICE(node,write,(char*)buf+off,sz)
	if(node->write)
		return node->write(node,buf,sz,off);
	else
		return -ENOSYS;
}

int fs_read(struct inode *node,void *buf,size_t sz,off_t off)
{
	FIX_DEVICE(node,read,(char*)buf+off,sz)
	if(node->read)
		return node->read(node,buf,sz,off);
	else
		return -ENOSYS;
}

#define	CHECK_DIR(x) \
	if(x->flags&0x7!=FS_DIR){ \
		errno=ENOTDIR; \
		return NULL; \
	} \

struct dirent *fs_readdir(struct inode *node,size_t index)
{
	CHECK_DIR(node)
	if(node->readdir)
		return node->readdir(node,index);
	else
		return NULL;
}

struct inode *fs_finddir(struct inode *node,char *name)
{
	CHECK_DIR(node)
	if(node->finddir)
		return node->finddir(node,name);
	else
		return NULL;
}
