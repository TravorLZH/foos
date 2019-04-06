#include <cpu/memory.h>
#include <cpu/interrupt.h>
#include <foos/system.h>
#include <foos/kmalloc.h>
#include <foos/debug.h>
#include <stdio.h>
#include <string.h>

#define	TABLE_SIZE	1024*sizeof(uint32_t)
#define	DIR_SIZE	1024*sizeof(uint32_t)

uint32_t *tables=NULL;

extern void vmem_enable(uint32_t *dir);

static void page_fault(struct registers regs)
{
	void *addr=NULL;
	uint8_t present=regs.err_code & 0x1;
	uint8_t write=regs.err_code & 0x2;
	uint8_t user=regs.err_code & 0x4;
	__asm__("movl %%cr2,%0":"=r"(addr));
	printf("Page fault occurs while %s on a%s page at 0x%x\n",
			write?"writing":"reading",
			present?"\0":" non-present",addr);
	if(user){
		printf("By the way, you accessed a kernel page\n");
	}
	do_stack_trace();
	hang();
}

int vmem_init(void *reserved)
{
	if(tables==NULL){
		tables=kmalloca(DIR_SIZE,PAGE_ALIGN);
		memset(tables,0,DIR_SIZE);
		__asm__("movl %%eax,%%cr3"::"a"(tables));
	}
	size_t i;
	for(i=0;i<4096;i++){
		void *addr=(void*)(i*PAGE_SIZE);
		pmem_mapaddr(addr,NULL,P_WRITABLE,tables);
	}
	int_hook_handler(0x0E,page_fault);
	vmem_enable(tables);
	return 0;
}

uint32_t *vmem_get(void *addr,void *dirptr)
{
	uint32_t *dir=(uint32_t*)dirptr;
	if(dir==NULL){
		__asm__("movl %%cr3,%%eax":"=a"(dir));
	}
	size_t tmp=(size_t)addr/PAGE_SIZE;
	size_t table_i=tmp/1024;
	size_t offset=tmp%1024;
	uint32_t *table=(uint32_t*)(dir[table_i] & (0xFFFFFFFF << PAGE_ALIGN));
	if(!(dir[table_i] & P_PRESENT) || table==NULL){
		table=(uint32_t*)kmalloca(TABLE_SIZE,PAGE_ALIGN);
		dir[table_i]=(size_t)table | P_PRESENT | P_WRITABLE;
	}
	return table+offset;
}
