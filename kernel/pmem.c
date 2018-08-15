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

int pmem_map(struct page *pg,void *physaddr,uint8_t flags)
{
	pg->present=1;
	pg->writable=flags & 0x1;
	pg->user=(flags & 0x2) >> 1;
	pg->frame=(size_t)physaddr >> PAGE_ALIGN;
	pmem_set(physaddr);
	return 0;
}

int pmem_mapaddr(void *virtaddr,void *physaddr,struct page_table *dir,
		uint8_t flags)
{
	struct page *pg=vmem_get(virtaddr,dir);
	return pmem_map(pg,physaddr,flags);
}
