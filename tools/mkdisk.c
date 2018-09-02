/* Create a ramdisk for FOOS
 * Program Usage: tools/mkdisk ramdisk.img disk/
 */
#define	_BSD_SOURCE
#include "../include/dev/ramdisk.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <unistd.h>

#define	die(...)	fprintf(stderr,__VA_ARGS__);exit(-1)

#define	MAX_NFILES	64

FILE *out=NULL;
struct rd_header *header=NULL;
struct rd_fileheader *files=NULL;
uint32_t offset=0;

void handle_directory(const char *dirname)
{
	FILE *tmp=NULL;
	DIR *indir=NULL;
	struct dirent *entry=NULL;
	struct rd_fileheader *ptr=files;
	if(chdir(dirname)){
		perror(dirname);
	}
	if((indir=opendir("."))==NULL){
		perror("Could not open directory");
		fclose(out);
		exit(-1);
	}
	while(entry=readdir(indir)){
		if(entry->d_type!=DT_REG){
			continue;
		}
		if((tmp=fopen(entry->d_name,"rb"))==NULL){
			perror(entry->d_name);
			closedir(indir);
			exit(-1);
		}
		ptr->magic=RD_FILESIG;
		strncpy(ptr->name,entry->d_name,64);
		fseek(tmp,0,SEEK_END);
		ptr->size=ftell(tmp);
		rewind(tmp);
		ptr->offset=offset;
		offset+=ptr->size;
		ptr++;
		fclose(tmp);
		header->nfiles++;
	}
	if(errno!=0){
		perror("Error on reading directory");
		closedir(indir);
		exit(-1);
	}
	closedir(indir);
}

int main(int argc,char **argv)
{
	int i;
	FILE *tmp=NULL;
	char *buf=NULL;
	uint32_t size,size2;
	if(argc<3){
		die("error: Too few arguments\n");
	}
	/* Open the output file before we change directory */
	if((out=fopen(argv[1],"wb"))==NULL){
		perror("Could not open output file for writing");
		exit(-1);
	}
	/* Allocate the data structures for RAM Disk */
	header=(struct rd_header*)malloc(sizeof(struct rd_header));
	memset(header,0,sizeof(struct rd_header));
	header->signature=RD_HEADSIG;
	files=(struct rd_fileheader*)
		malloc(MAX_NFILES*sizeof(struct rd_fileheader));
	memset(files,0,MAX_NFILES*sizeof(struct rd_fileheader));
	handle_directory(argv[2]);
	uint32_t base=header->nfiles * sizeof(struct rd_fileheader);
	fwrite(header,sizeof(struct rd_header),1,out);
	fwrite(files,base,1,out);
	for(i=0;i<header->nfiles;i++){
		files[i].offset+=base;
		tmp=fopen(files[i].name,"rb");
		buf=malloc(files[i].size);
		fread(buf,1,files[i].size,tmp);
		fwrite(buf,1,files[i].size,out);
		fclose(tmp);
		free(buf);
		printf("%s: length=%u, offset=%u\n",files[i].name,
				files[i].size,files[i].offset);
	}
	size=ftell(out);
	/* We align this file by sectors (512 bytes) */
	if(size%0x200){
		size2=size;
		size2&=0xFFFFFE00;
		size2+=0x200;
	}
	size2-=size;
	for(i=0;i<size2;i++){
		char c=0;
		fwrite(&c,1,1,out);
	}
	free(header);
	free(files);
	fclose(out);
	return 0;
}
