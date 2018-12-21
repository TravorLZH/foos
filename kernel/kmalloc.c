/* A home-brewed memory allocator for FOOS
 *
 * It is based on a linked list, which points to the next block
 *
 * The block will be used if its size is greater than or equal to requested one
 * New blocks will be created if no blocks fit the size
 *
 * It splits the block if the block is too large for the request
 *
 * This allocator helps zero the memory for you during allocation
 *
 * CAUTION: Don't ever access RAM across the border
 */
#include <foos/kmalloc.h>
#include <foos/system.h>
#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>

/* Meta-data for each allocated block */
#define	BLOCKSIZ	16

struct block {
	uint32_t used;
	size_t size;
	struct block *prev;
	struct block *next;
	char data[1];
};

static struct block *root=(struct block*)HEAP_BASE;

static void merge(struct block *blk)
{
	if(blk->next && blk->next->used==0){
		blk->size+=BLOCKSIZ+blk->next->size;
		blk->next=blk->next->next;
		blk->next->prev=blk;
	}
}

static void split(struct block *blk,size_t size)
{
	assert(blk!=NULL);
	size_t delta=blk->size - size;
	struct block *new=(struct block*)(blk->data + size);
	if(blk->next==NULL || delta<=BLOCKSIZ+8){
		return;
	}
	new->next=blk->next;
	new->size=(char*)new->next - (char*)new->data;
	new->next->prev=new;
	new->prev=blk;
	new->used=0;
	blk->next=new;
	blk->size=size;
}

static struct block *lookup(struct block *blk,size_t size,size_t align)
{
	size_t x=0xFFFFFFFF << align;
	size_t ax=1 << align;
	struct block **ptr= (blk==NULL) ? &root : &blk->next;
	if(*ptr==NULL){
		*ptr=(struct block*)((char*)blk + BLOCKSIZ + blk->size);
		if(align && (size_t)(*ptr)->data%ax){
			*ptr = (struct block*)((size_t)*ptr & x);
			*ptr = (struct block*)((char*)*ptr + ax - BLOCKSIZ);
			blk->size=(char*)*ptr - blk->data;
		}
		(*ptr)->prev=blk;
		(*ptr)->size=size;
	}
	if((*ptr)->size >= size && (*ptr)->used==0){
		/* If the address is not aligned */
		if(align && (size_t)(*ptr)->data%ax){
			return lookup(*ptr,size,align);
		}
		(*ptr)->used=1;
		split(*ptr,size);
		return *ptr;
	}
	if((*ptr)->size == 0 && !align){
		(*ptr)->size=size;
		(*ptr)->used=1;
		return *ptr;
	}
	return lookup(*ptr,size,align);
}

void *kmalloca(size_t size,size_t align)
{
	assert(root!=NULL);
	if(size==0){
		errno=EINVAL;
		return NULL;
	}
	/* Align the size to eight */
	size=(size & 0x7) ? ((size >> 3) + 1) << 3 : size;
	struct block *ptr=lookup(NULL,size,align);
	if(ptr==NULL){
		errno=ENOMEM;
	}
	return ptr->data;
}

void *kmalloc(size_t size)
{
	return kmalloca(size,0);
}

void kfree(void *ptr)
{
	assert(ptr > (void*)HEAP_BASE);
	struct block *blk=(struct block*)((char*)ptr - BLOCKSIZ);
	blk->used=0;
	if(blk->prev && blk->prev->used==0){
		merge(blk->prev);
	}
	merge(blk);
}

static void do_block(struct block *blk)
{
	printf("block at 0x%x: used=%d, size=%d\n",blk,blk->used,blk->size);
	if(blk->next){
		do_block(blk->next);
	}
}

void kmalloc_info(void)
{
	do_block(root);
}
