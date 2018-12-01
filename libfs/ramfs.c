#include <foos/fs.h>
#include <foos/device.h>
#include <foos/ramfs.h>
#include <dev/ramdisk.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

static struct rd_header *header=NULL;
static struct rd_fileheader *files=NULL;
static struct inode root;
static size_t rd_size;
struct dirent dirent;

int ramfs_init(void)
{
	char buf[128];
	size_t base;
	int i;
	assert(!dev_ioctl(DEV_RAMDISK,RD_GETADDR,&header));
	files=(struct rd_fileheader*)(header+1);
	if(header->signature!=RD_HEADSIG){
		puts("Invalid Signature");
		return -1;
	}
	base=header->nfiles;
	base*=sizeof(struct rd_fileheader);
	base+=sizeof(struct rd_header);
	for(i=0;i<header->nfiles;i++){
		assert(files[i].magic==RD_FILESIG);
		printf("Found file `%s'\n",files[i].name);
		dev_seek(DEV_RAMDISK,base+files[i].offset,SEEK_SET);
		dev_read(DEV_RAMDISK,buf,files[i].size);
		buf[files[i].size]='\0';
		printf("Content: `%s'\n",buf);
	}
	return 0;
}

struct dirent *ramfs_readdir(struct inode *node,size_t index)
{
	strcpy(dirent.name,files[index].name);
	dirent.ino=index;
	return &dirent;
}
