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
