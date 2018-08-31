#include <foos/kmalloc.h>
#include <foos/system.h>
#include <inttypes.h>

char *heap=(char *)HEAP_BASE;

void *kmalloc(size_t sz)
{
	void *ret=heap;
	heap+=sz;
	return ret;
}

void *kmalloca(size_t sz,size_t align)
{
	size_t tmp=(size_t)heap;
	size_t x=0xFFFFFFFF << align;
	align=1 << align;
	if(tmp%align)
	{
		tmp&=x;
		tmp+=align;
		heap=(char*)tmp;
	}
	return kmalloc(sz);
}

void kfree(void *ptr)
{
	// For future use
}
