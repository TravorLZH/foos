#include <foos/device.h>
#include <dev/tty.h>
#include <dev/ramdisk.h>
#include <foos/system.h>
#include <errno.h>
#include <stdio.h>

struct device devs[]={
	{
		"tty",0,NULL,
		ttydev_write,
		ttydev_read,
		ttydev_open,
		ttydev_close,
		ttydev_ioctl
	},
	{
		"ramdisk",0,NULL,
		NULL,
		ramdisk_read,
		ramdisk_open,
		ramdisk_close,
		ramdisk_ioctl
	}
};

size_t ndevs=ARRAY_SIZE(devs);

size_t dev_write(int no,const void *buf,size_t len)
{
	struct device *ptr=devs+no;
	if(ptr->write!=NULL){
		return ptr->write(ptr,buf,len);
	}
	errno=ENOSYS;
	return -ENOSYS;
}

size_t dev_read(int no,void *buf,size_t len)
{
	struct device *ptr=devs+no;
	if(ptr->read!=NULL){
		int ret=ptr->read(ptr,buf,ptr->offset,len);
		if(ret>0){
			ptr->offset+=ret;
		}
		return ret;
	}
	errno=ENOSYS;
	return -ENOSYS;
}

int dev_open(int no,int flags)
{
	struct device *ptr=devs+no;
	ptr->offset=0;
	if(ptr->open!=NULL){
		return ptr->open(ptr,flags);
	}
	errno=ENOSYS;
	return -ENOSYS;
}

int dev_close(int no)
{
	struct device *ptr=devs+no;
	ptr->offset=0;
	if(ptr->close!=NULL){
		printf("dev: Closing `%s'\n",ptr->name);
		return ptr->close(ptr);
	}
	printf("dev: warning: `%s' is not closable\n",ptr->name);
	errno=ENOSYS;
	return -ENOSYS;
}

int dev_ioctl(int no,int request,void *args)
{
	struct device *ptr=devs+no;
	if(ptr->ioctl!=NULL){
		return ptr->ioctl(ptr,request,args);
	}
	errno=ENOSYS;
	return -ENOSYS;
}

size_t dev_seek(int no,size_t off,int whence)
{
	struct device *ptr=devs+no;
	switch(whence){
	case SEEK_SET:
		ptr->offset=off;
		break;
	case SEEK_CUR:
		ptr->offset+=off;
		break;
	default:
		errno=EINVAL;
		return -EINVAL;
	}
	return ptr->offset;
}
