#ifndef	DEBUG_H
#define	DEBUG_H

#include <inttypes.h>

struct stack_frame {
	struct stack_frame *ebp;
	uint32_t eip;
};

struct symtable {
	uint32_t magic;
	uint32_t addr;
	char symbol[32];
};

void do_stack_trace(void);

#endif
