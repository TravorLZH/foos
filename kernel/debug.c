#include <dev/serial.h>
#include <foos/debug.h>

void do_stack_trace(void)
{
	struct stack_frame *frm;
	int i,j;
	__asm__("movl %%ebp,%0":"=r"(frm));
	for(i=0;frm->ebp;frm=frm->ebp,i++){
		serial_print("[trace] ");
		for(j=0;j<i;j++)
			serial_send('\t');
		serial_printf("\\-- frame 0x%x\n",frm->eip);
	}
}
