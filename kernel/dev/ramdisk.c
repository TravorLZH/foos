#include <foos/kmalloc.h>
#include <dev/ramdisk.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

struct rd_header *header=NULL;
struct rd_fileheader *files=NULL;

int ramdisk_init(void *addr,uint32_t size)
{
	char *base=NULL;
	header=(struct rd_header*)addr;
	files=(struct rd_fileheader*)(header+1);
	int i;
	char buf[128];
	if(header->signature!=RD_HEADSIG){
		puts("Invalid Signature");
		return -1;
	}
	base=(char*)(files+header->nfiles);
	for(i=0;i<header->nfiles;i++){
		assert(files[i].magic==RD_FILESIG);
		printf("Found file `%s'\n",files[i].name);
		memset(buf,0,sizeof(buf));
		memcpy(buf,base+files[i].offset,files[i].size);
		printf("Content: `%s'\n",buf);
	}
	return 0;
}

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
	case RD_FLUSH:
		return ramdisk_init(rd->addr,rd->size);
	default:
		return EINVAL;
	}
	return 0;
}

size_t ramdisk_read(struct device *dev,void *buf,size_t off,size_t len)
{
	struct ramdisk *rd=dev->data;
	if(rd==NULL){
		return EINVAL;
	}
	memcpy((char*)buf+off,(char*)rd->addr+off,len);
	return len;
}
