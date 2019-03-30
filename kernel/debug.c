#include <dev/serial.h>
#include <foos/debug.h>

void do_stack_trace(void)
{
	struct stack_frame *frm;
	__asm__("movl %%ebp,%0":"=r"(frm));
	for(;frm->ebp;frm=frm->ebp)
		serial_printf("Frame addr: 0x%x\n",frm->eip);
}
