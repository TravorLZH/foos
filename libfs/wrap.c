#include <foos/fs.h>

int fs_open(struct inode *node,int flags)
{
	if(node->open)
		return node->open(node,flags);
	else
		return 0;
}

int fs_close(struct inode *node)
{
	if(node->close)
		return node->close(node);
	else
		return 0;
}

int fs_write(struct inode *node,const void *buf,size_t sz,off_t off)
{
	if(node->write)
		return node->write(node,buf,sz,off);
	else
		return 0;
}

int fs_read(struct inode *node,void *buf,size_t sz,off_t off)
{
	if(node->read)
		return node->read(node,buf,sz,off);
	else
		return 0;
}
