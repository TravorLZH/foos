#include <foos/fs.h>
#include <foos/kmalloc.h>
#include <foos/device.h>
#include <foos/ramfs.h>
#include <dev/ramdisk.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

static struct inode *rd_files=NULL;
static size_t file_base;
static size_t rd_nfiles;
static size_t rd_size;
static struct inode *rd_root;
struct dirent dirent;

struct inode *ramfs_init(void)
{
	struct rd_header *header;
	struct rd_fileheader *files;
	int i;
	assert(!dev_ioctl(DEV_RAMDISK,RD_GETADDR,&header));
	files=(struct rd_fileheader*)(header+1);
	if(header->signature!=RD_HEADSIG){
		puts("ramfs: Bad signature");
		return NULL;
	}
	file_base=rd_nfiles=header->nfiles;
	file_base*=sizeof(struct rd_fileheader);
	file_base+=sizeof(struct rd_header);
	puts("ramfs: Creating inode for `/'");
	rd_root=(struct inode*)kmalloc(sizeof(struct inode));
	strcpy(rd_root->name,"ramdisk");
	rd_root->flags=FS_DIR;
	rd_root->readdir=ramfs_readdir;
	rd_root->finddir=ramfs_finddir;
	rd_root->ptr=NULL;
	rd_files=kmalloc(header->nfiles * sizeof(struct inode));
	for(i=0;i<rd_nfiles;i++){
		assert(files[i].magic==RD_FILESIG);
		printf("ramfs: Creating inode for `%s'\n",files[i].name);
		strcpy(rd_files[i].name,files[i].name);
		rd_files[i].ino=i;
		rd_files[i].impl=files[i].offset;
		rd_files[i].size=files[i].size;
		rd_files[i].read=ramfs_read;
	}
	return rd_root;
}

struct dirent *ramfs_readdir(struct inode *node,size_t index)
{
	assert(node==rd_root);
	if(index>=rd_nfiles){
		return NULL;
	}
	strcpy(dirent.name,rd_files[index].name);
	dirent.ino=index;
	return &dirent;
}

struct inode *ramfs_finddir(struct inode *node,char *name)
{
	int i;
	for(i=0;i<rd_nfiles;i++){
		if(!strcmp(rd_files[i].name,name)){
			return rd_files+i;
		}
	}
	return NULL;
}

int ramfs_read(struct inode *node,void *buf,size_t sz,off_t offset)
{
	dev_seek(DEV_RAMDISK,file_base+node->impl,SEEK_SET);
	dev_read(DEV_RAMDISK,(char*)buf + offset,node->size);
	return sz;
}
