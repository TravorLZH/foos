#include <foos/fs.h>
#include <foos/device.h>
#include <foos/ramfs.h>
#include <dev/ramdisk.h>
#include <assert.h>
#include <stdio.h>

static struct rd_header *header=NULL;
static struct rd_fileheader *files=NULL;
static struct inode rd_root;
static size_t file_base;
static size_t rd_size;
struct dirent dirent;

static char *strcpy(char *to,const char *from)
{
	char *s=to;
	while(*s++=*from++);
	return to;
}

struct inode *ramfs_init(void)
{
	char buf[128];
	int i;
	assert(!dev_ioctl(DEV_RAMDISK,RD_GETADDR,&header));
	files=(struct rd_fileheader*)(header+1);
	if(header->signature!=RD_HEADSIG){
		puts("ramfs: Bad signature");
		return -1;
	}
	file_base=header->nfiles;
	file_base*=sizeof(struct rd_fileheader);
	file_base+=sizeof(struct rd_header);
//	for(i=0;i<header->nfiles;i++){
//		assert(files[i].magic==RD_FILESIG);
//		printf("Found file `%s'\n",files[i].name);
//		dev_seek(DEV_RAMDISK,file_base+files[i].offset,SEEK_SET);
//		dev_read(DEV_RAMDISK,file_buf,files[i].size);
//		buf[files[i].size]='\0';
//		printf("Content: `%s'\n",buf);
//	}
	puts("ramfs: Creating node for path `/'");
	strcpy(rd_root.name,"ramdisk");
	rd_root.mask=rd_root.gid=rd_root.uid=rd_root.ino=rd_root.size=0;
	rd_root.flags=FS_DIR;
	rd_root.read=rd_root.write=rd_root.open=rd_root.close=NULL;
	rd_root.readdir=&ramfs_readdir;
	rd_root.finddir=NULL;
	rd_root.ptr=NULL;
	return &rd_root;
}

struct dirent *ramfs_readdir(struct inode *node,size_t index)
{
	if(index>=header->nfiles){
		return NULL;
	}
	strcpy(dirent.name,files[index].name);
	dirent.ino=index;
	return &dirent;
}
