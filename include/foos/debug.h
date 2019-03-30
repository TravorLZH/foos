#ifndef	DEBUG_H
#define	DEBUG_H

#include <inttypes.h>

struct stack_frame {
	struct stack_frame *ebp;
	uint32_t eip;
};

void do_stack_trace(void);

#endif
