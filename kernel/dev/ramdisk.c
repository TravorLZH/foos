#include <foos/kmalloc.h>
#include <dev/ramdisk.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int ramdisk_open(struct device *dev,int flags)
{
	if(dev->data==NULL){
		dev->data=kmalloc(sizeof(struct ramdisk));
		return 0;
	}
	errno=EBUSY;
	return -EBUSY;
}

int ramdisk_close(struct device *dev)
{
	if(dev->data!=NULL){
		kfree(dev->data);
	}
	return 0;
}

int ramdisk_ioctl(struct device *dev,int request,void *arg)
{
	struct ramdisk *rd=dev->data;
	if(rd==NULL){
		return EINVAL;
	}
	switch(request){
	case RD_SETADDR:
		rd->addr=*(void**)arg;
		break;
	case RD_GETADDR:
		*(void**)arg=rd->addr;
		break;
	case RD_SETSIZE:
		rd->size=*(size_t*)arg;
		break;
	case RD_GETSIZE:
		*(size_t*)arg=rd->size;
		break;
	default:
		return EINVAL;
	}
	return 0;
}

size_t ramdisk_read(struct device *dev,void *buf,size_t off,size_t len)
{
	struct ramdisk *rd=dev->data;
	char *src=(char*)rd->addr+off;
	if(rd==NULL){
		errno=EINVAL;
		return -EINVAL;
	}
	memcpy(buf,src,len);
	return len;
}
