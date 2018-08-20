#include <cpu/memory.h>

int liballoc_lock(void)
{
	// TODO: Spinlock
	return 0;
}

int liballoc_unlock(void)
{
	// TODO: Unlock
	return 0;
}

void *liballoc_alloc(int n){
	return vmem_alloc(n);
}

int liballoc_free(void *addr,int n)
{
	vmem_free(addr,n);
	return 0;
}
