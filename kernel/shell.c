/*
 * This is just a kernel shell that has a few commands to do certain things
 * (e.g. Get status of floppy, list root directories, print file content)
 */
#include <foos/fs.h>
#include <foos/kmalloc.h>
#include <asm/cmos.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

extern char *buf;
extern int _puts(const char*);
extern void kmalloc_info(void);

static const char* floppy_type[]={
	"Not Applicable",
	"360 KB 5.25\"",
	"1.2 MB 5.25\"",
	"720 KB 3.5\"",
	"1.44 MB 3.5\"",
	"2.88 MB 3.5\""
};

static const char* file_type[]={
	NULL,"file","dir","chardev","blkdev","pipe"
};

static const char hello_world[]="Hello world\n";

static void check_floppy(void)
{
	uint8_t val=cmos_read(0x10);
	_puts("fd0: ");
	puts(floppy_type[(val >> 4) & 0xF]);
	_puts("fd1: ");
	puts(floppy_type[val & 0xF]);
}

static void check_memory(void)
{
	uint32_t low_mem=cmos_read(0x16) << 8;
	low_mem |= cmos_read(0x15);
	printf("low memory: 0 - 0x%x\n",low_mem << 10);
	kmalloc_info();
}

static void cat_file(char *ptr)
{
	struct inode *file=fs_finddir(fs_root,ptr);
	if(file==NULL){
		printf("%s: No such file or directory\n",ptr);
		return;
	}else if(file->flags & FS_DIR){
		printf("%s: Is a directory\n",ptr);
		return;
	}
	fs_open(file,0);
	fs_read(file,buf,file->size,0);
	puts(buf);
	fs_close(file);
}

static void list_directory(struct inode *dir)
{
	int i;
	struct dirent *ent=NULL;
	struct inode *tmp=NULL;
	if(dir==NULL){
		puts("No such file or directory");
		return;
	}
	if((dir->flags & 0x7)!=FS_DIR){
		printf("%s: Not a directory\n",dir->name);
		return;
	}
	printf("%d files in `%s':\n",dir->impl,dir->name);
	for(i=0;i<dir->impl;i++){
		ent=fs_readdir(dir,i);
		tmp=fs_finddir(dir,ent->name);
		printf("%s\t%s\t%d bytes\n",file_type[tmp->flags & 0xF],
				tmp->name,tmp->size);
	}
}

static void check_time(void)
{
	/* These are BCDs */
	uint16_t year=cmos_read(0x32) << 8;
	year |= cmos_read(0x09);
	uint8_t month=cmos_read(0x08);
	uint8_t day=cmos_read(0x07);
	uint8_t hour=cmos_read(0x04);
	uint8_t minute=cmos_read(0x02);
	uint8_t second=cmos_read(0x00);
	printf("%x/%x/%x %x:%x:%x\n",year,month,day,hour,minute,second);
}

static void list_devs(void)
{
	struct inode *devdir=fs_finddir(fs_root,"dev");
	int i=0;
	struct dirent *dev;
	puts("FOOS devices:");
	while(dev=fs_readdir(devdir,i++)){
		printf("%d: %s\n",dev->ino,dev->name);
	}
}

static void test_fsdev(void)
{
	struct inode *tty=fs_lookup("/dev/tty");
	if(tty==NULL){
		puts("tty not found!");
		return;
	}
	fs_write(tty,hello_world,sizeof(hello_world)-1,0);
}

static void test_strtok(void)
{
	char test[]="/dev/tty";
	char delim[]="/";
	char *ptr=NULL;
	printf("Tokenizing `%s' by `%s'\n",test,delim);
	puts(strtok(test,delim));
	while(ptr=strtok(NULL,delim))
		puts(ptr);
}

static void test_malloc(void)
{
	char *a=(char*)kmalloc(10);
	printf("kmalloc(10): 0x%x\n",a);
	kfree(a);
	printf("kfree(0x%x)\n",a);
	a=(char*)kmalloc(10);
	printf("kmalloc(10) again: 0x%x\n",a);
	kfree(a);
}

static void trigger_pf(void)
{
	char *a=(char*)0xFFFFFFFF;
	*a=0;
}

static void shell_help(void);

struct cmd_handler {
	char cmd[16];
	char description[32];
	void (*handle)(void);
};

struct cmd_handler handlers[]={
	{"help","print this message",shell_help},
	{"ls [FILE]","list directory contents",NULL},
	{"cat [FILE]","print out the contents of a file",NULL},
	{"devs","list available FOOS devices",list_devs},
	{"floppy","check floppy status",check_floppy},
	{"time","get the current time",check_time},
	{"mem","print the status of kernel heap",check_memory},
	{"tty","write `Hello world' to /dev/tty",test_fsdev},
	{"tok","test tokenizing strings",test_strtok},
	{"malloc","test the efficiency of malloc()",test_malloc},
	{"fault","trigger a page fault",trigger_pf},
	{"exit","quit the shell",NULL}
};

int nhandlers=ARRAY_SIZE(handlers);

static void shell_help(void)
{
	int i;
	for(i=0;i<nhandlers;i++)
		printf("%s - %s\n",handlers[i].cmd,handlers[i].description);
}

/* TODO: This should become a separate process after multitasking implemented */
int shell_main(void)
{
	int i;
	putchar('\n');
loop:
	_puts("> ");
	memset(buf,0,BUFSIZ);
	gets(buf);
	if(!strcmp(buf,"exit")){
		return 0;	// Quit the shell and probably halts the system
	}
	if(!memcmp(buf,"ls",2)){
		if(strlen(buf)<=3){
			list_directory(fs_root);
		}else{
			list_directory(fs_lookup(buf+3));
		}
		goto loop;
	}
	if(!memcmp(buf,"cat ",4)){
		cat_file(buf+4);
		goto loop;
	}
	for(i=0;i<nhandlers;i++){
		if(!strcmp(buf,handlers[i].cmd)){
			if(handlers[i].handle)
				handlers[i].handle();
			else
				printf("No handler found for `%s'\n",
						handlers[i].cmd);
			goto loop;
		}
	}
	if(buf[0]!='\0'){
		printf("%s: Unknown command\n",buf);
	}
	goto loop;
	return 0;
}
