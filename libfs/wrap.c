#include <foos/system.h>
#include <foos/fs.h>
#include <foos/device.h>
#include <foos/kmalloc.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define	FIX_DEVICE(x,f,...) \
	if((x->flags & 0x7)==FS_CHAR||(x->flags & 0x7)==FS_BLK){ \
		return dev_##f(x->impl,__VA_ARGS__); \
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
	if(node->write)
		return node->write(node,buf,sz,off);
	else
		return -ENOSYS;
}

int fs_read(struct inode *node,void *buf,size_t sz,off_t off)
{
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

struct inode *fs_finddir(struct inode *node,const char *name)
{
	CHECK_DIR(node)
	if(node->finddir)
		return node->finddir(node,name);
	else
		return NULL;
}

char file_separator[]="/";

struct inode *fs_lookup(const char *path)
{
	assert(fs_root != NULL);
	assert(path != NULL);
	assert(strlen(path) > 0);
	struct inode *node=fs_root;
	int size=strlen(path)+1;
	char *tmp=kmalloc(size);
	memset(tmp,0,size);
	char *ptr=NULL;
	strcpy(tmp,path);
	ptr=strtok(tmp,file_separator);
	if(ptr==NULL){
		kfree(tmp);
		return fs_finddir(node,path);
	}
	node=fs_finddir(node,ptr);
	while((ptr=strtok(NULL,file_separator))!=NULL && node!=NULL){
		node=fs_finddir(node,ptr);
	}
	kfree(tmp);
	return node;
}
