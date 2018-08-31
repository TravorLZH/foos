#include <foos/device.h>
#include <foos/system.h>
#include <errno.h>

extern size_t ttydev_write(struct device *dev,const void *buf,size_t len);
extern size_t ttydev_read(struct device *dev,void *buf,size_t len);
extern int ttydev_open(struct device *dev,uint8_t flags);
extern int ttydev_close(struct device *dev);

struct device devs[]={
	{
		"tty",(devwrite_t)ttydev_write,
		(devread_t)ttydev_read,
		(devopen_t)ttydev_open,
		(devclose_t)ttydev_close
	}
};

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
		return ptr->read(ptr,buf,len);
	}
	errno=ENOSYS;
	return -ENOSYS;
}

int dev_open(int no,uint8_t flags)
{
	struct device *ptr=devs+no;
	if(ptr->open!=NULL){
		return ptr->open(ptr,flags);
	}
	errno=ENOSYS;
	return -ENOSYS;
}

int dev_close(int no)
{
	struct device *ptr=devs+no;
	if(ptr->close!=NULL){
		return ptr->close(ptr);
	}
	errno=ENOSYS;
	return -ENOSYS;
}
