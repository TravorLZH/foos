#include <foos/fs.h>
#include <foos/devfs.h>
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
static struct dirent dirent;

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
	memset(rd_root,0,sizeof(struct inode));
	strcpy(rd_root->name,"ramdisk");
	rd_root->flags=FS_DIR;
	rd_root->readdir=ramfs_readdir;
	rd_root->finddir=ramfs_finddir;
	rd_files=kmalloc((header->nfiles+1) * sizeof(struct inode));
	rd_root->ptr=rd_files;
	memset(rd_files,0,(header->nfiles+1) * sizeof(struct inode));
	for(i=0;i<rd_nfiles;i++){
		assert(files[i].magic==RD_FILESIG);
		printf("ramfs: Creating inode for `%s'\n",files[i].name);
		strcpy(rd_files[i].name,files[i].name);
		rd_files[i].flags=FS_FILE;
		rd_files[i].ino=i;
		rd_files[i].impl=files[i].offset;
		rd_files[i].size=files[i].size;
		rd_files[i].read=ramfs_read;
	}
	/* Create directory for device */
	rd_nfiles++;
	rd_files[i].ino=i;
	rd_files[i].finddir=ramfs_finddir;
	rd_files[i].readdir=ramfs_readdir;
	devfs_init(&rd_files[i]);

	rd_root->impl=rd_nfiles;
	return rd_root;
}

struct dirent *ramfs_readdir(struct inode *node,size_t index)
{
	if(index>=node->impl){
		return NULL;
	}
	strcpy(dirent.name,node->ptr[index].name);
	dirent.ino=index;
	return &dirent;
}

struct inode *ramfs_finddir(struct inode *node,char *name)
{
	int i;
	for(i=0;i<node->impl;i++){
		if(!strcmp(node->ptr[i].name,name)){
			return node->ptr+i;
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
