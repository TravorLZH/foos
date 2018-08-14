#include <cpu/memory.h>
#include <cpu/interrupt.h>
#include <foos/system.h>
#include <foos/kmalloc.h>
#include <stdio.h>
#include <string.h>

#define	TABLE_SIZE	1024*sizeof(struct page)
#define	DIR_SIZE	1024*sizeof(struct page_table)

struct page *kernel_table=NULL;
struct page *kernel_heap=NULL;
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
	if(kernel_table==NULL){
		kernel_table=kmalloca(TABLE_SIZE,PAGE_ALIGN);
		memset(kernel_table,0,TABLE_SIZE);
	}
	if(kernel_heap==NULL){
		kernel_heap=kmalloca(TABLE_SIZE,PAGE_ALIGN);
		memset(kernel_heap,0,TABLE_SIZE);
	}
	if(tables==NULL){
		tables=kmalloca(DIR_SIZE,PAGE_ALIGN);
		memset(tables,0,DIR_SIZE);
	}
	size_t i;
	for(i=0;i<1024;i++){
		kernel_table[i].present=1;
		kernel_table[i].writable=1;
		kernel_table[i].user=0;
		kernel_table[i].frame=i;
	}
	for(i=0;i<1024;i++){
		kernel_heap[i].present=1;
		kernel_heap[i].writable=1;
		kernel_heap[i].user=0;
		kernel_heap[i].frame=i+1024;
	}
	tables[0].present=1;
	tables[0].writable=1;
	tables[0].user=0;
	tables[0].page=(size_t)kernel_table >> PAGE_ALIGN;
	tables[1].present=1;
	tables[1].writable=1;
	tables[1].user=0;
	tables[1].page=(size_t)kernel_heap >> PAGE_ALIGN;
	int_hook_handler(0x0E,page_fault);
	__asm__("movl %%eax,%%cr3"::"a"(tables));
	__asm__("movl %%cr0,%%eax\n"
		"orl $0x80000000,%%eax\n"
		"movl %%eax,%%cr0":::"eax");
	return 0;
}
