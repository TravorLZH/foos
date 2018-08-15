#include <cpu/memory.h>
#include <foos/system.h>
#include <foos/kmalloc.h>

#define	FRAMES_SIZE	(NPAGES/sizeof(uint32_t))

#define	BIT_INDEX(x)	((x)/32)
#define	BIT_OFFSET(x)	((x)%32)

static uint32_t *frame_set=NULL;

void pmem_set(void *addr)
{
	size_t frame=(size_t)addr/PAGE_SIZE;
	frame_set[BIT_INDEX(frame)]|=1 << BIT_OFFSET(frame);
}

void pmem_clear(void *addr)
{
	size_t frame=(size_t)addr/PAGE_SIZE;
	frame_set[BIT_INDEX(frame)]&=~(1 << BIT_OFFSET(frame));
}

int pmem_test(void *addr)
{
	size_t frame=(size_t)addr/PAGE_SIZE;
	return frame_set[BIT_INDEX(frame)] & (1 << BIT_OFFSET(frame));
}

int pmem_init(void *reserved)
{
	frame_set=(uint32_t*)kmalloc(PAGE_SIZE);
}

int pmem_map(uint32_t *pg,void *physaddr,uint32_t flags)
{
	if(physaddr==NULL){
		physaddr=pmem_get_free();
	}
	*pg|=(size_t)physaddr | P_PRESENT | flags;
	pmem_set(physaddr);
	return 0;
}

int pmem_mapaddr(void *virtaddr,void *physaddr,uint32_t flags,uint32_t *dir)
{
	uint32_t *pg=vmem_get(virtaddr,dir);
	return pmem_map(pg,physaddr,flags);
}

void *pmem_get_free(void)
{
	char *addr=(char*)0x0;
	for(;addr<(char*)MAPPED_MEMORY;addr+=PAGE_SIZE){
		if(!pmem_test(addr)){	// If it is a free frame
			return addr;
		}
	}
	return NULL;
}
