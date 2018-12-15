/*
 * This is just a kernel shell that has a few commands to do certain things
 * (e.g. Get status of floppy, list root directories, print file content)
 */
#include <foos/fs.h>
#include <foos/kmalloc.h>
#include <asm/cmos.h>
#include <stdio.h>
#include <string.h>

extern char *buf;
extern int _puts(const char*);

static const char* floppy_type[]={
	"Not Applicable",
	"360 KB 5.25\"",
	"1.2 MB 5.25\"",
	"720 KB 3.5\"",
	"1.44 MB 3.5\"",
	"2.88 MB 3.5\""
};

static void check_floppy(void)
{
	uint8_t val=cmos_read(0x10);
	_puts("fd0: ");
	puts(floppy_type[(val >> 4) & 0xF]);
	_puts("fd1: ");
	puts(floppy_type[val & 0xF]);
}

void cat_file(char *ptr)
{
	struct inode *file=fs_finddir(fs_root,ptr);
	if(file==NULL){
		printf("%s: No such file or directory\n",ptr);
		return;
	}
	fs_open(file,0);
	fs_read(file,buf,file->size,0);
	puts(buf);
	fs_close(file);
}

void list_directory(void)
{
	int i=0;
	struct dirent *ent=NULL;
	struct inode *tmp=NULL;
	ent=fs_readdir(fs_root,0);
	puts("Files in ramdisk: (name, size in bytes)");
	do{
		tmp=fs_finddir(fs_root,ent->name);
		printf("| %s\t| %d\n",tmp->name,tmp->size);
	}while(ent=fs_readdir(fs_root,++i));
}

int shell_main(void)
{
	putchar('\n');
loop:
	_puts("> ");
	memset(buf,0,BUFSIZ);
	gets(buf);
	if(!strcmp(buf,"ls")){
		list_directory();
		goto loop;
	}
	if(!strcmp(buf,"floppy")){
		check_floppy();
		goto loop;
	}
	if(!memcmp(buf,"cat ",4)){
		cat_file(buf+4);
		goto loop;
	}
	if(!strcmp(buf,"malloc")){
		char *a=(char*)kmalloc(10);
		printf("kmalloc(10): 0x%x\n",a);
		kfree(a);
		printf("kfree(0x%x)\n",a);
		a=(char*)kmalloc(10);
		printf("kmalloc(10) again: 0x%x\n",a);
		kfree(a);
		goto loop;
	}
	if(buf[0]!='\0'){
		printf("%s: Unknown command\n",buf);
	}
	goto loop;
	return 0;
}
