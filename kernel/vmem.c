#include <cpu/memory.h>
#include <cpu/interrupt.h>
#include <foos/system.h>
#include <foos/kmalloc.h>
#include <stdio.h>
#include <string.h>

#define	TABLE_SIZE	1024*sizeof(struct page)
#define	DIR_SIZE	1024*sizeof(struct page_table)

struct page_table *tables=NULL;

extern void vmem_enable(struct page_table *dir);

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
	hang();
}

int vmem_init(void *reserved)
{
	if(tables==NULL){
		tables=kmalloca(DIR_SIZE,PAGE_ALIGN);
		memset(tables,0,DIR_SIZE);
	}
	size_t i,j;
	for(i=0;i<1024;i++){
		for(j=0;j<1024;j++){
			void *addr=(void*)((i<<22)+(j<<PAGE_ALIGN));
			pmem_mapaddr(addr,addr,tables,P_WRITABLE);
		}
	}
	int_hook_handler(0x0E,page_fault);
	vmem_enable(tables);
	return 0;
}

struct page *vmem_get(void *addr,struct page_table *dir)
{
	if(dir==NULL){
		__asm__("movl %%cr3,%%eax":"=a"(dir));
	}
	size_t tmp=(size_t)addr/PAGE_SIZE;
	size_t table_i=tmp/1024;
	size_t offset=tmp%1024;
	struct page *table=(struct page*)(dir[table_i].page << PAGE_ALIGN);
	if(!dir[table_i].present || table==NULL){
		table=(struct page*)
			kmalloca(TABLE_SIZE,PAGE_ALIGN);
		dir[table_i].present=1;
		dir[table_i].writable=0;
		dir[table_i].user=0;
		dir[table_i].page=(size_t)table >> PAGE_ALIGN;
	}
	return table+offset;
}
