#ifndef	INTERRUPT_H
#define	INTERRUPT_H
#include <inttypes.h>

struct idt_entry{
	uint16_t base_lo;
	uint16_t segment;
	uint8_t zero;	// Padding
	uint8_t flags;
	uint16_t base_hi;
} __attribute__((packed));

struct idt_desc{
	uint16_t limit;
	void *base;
} __attribute__((packed));

struct registers{
	uint32_t ds;
	uint32_t edi,esi,ebp,junk,ebx,edx,ecx,eax;	// From pusha
	uint32_t int_no,err_code;
	uint32_t eip,cs,eflags,esp,ss;	// From processor
};

typedef	void (*inthandler_t)(struct registers);

extern void int_init(void);
extern void int_hook_handler(uint8_t no,inthandler_t handler);

/* Hooks of each interrupts */
extern void int0(void);
extern void int1(void);
extern void int2(void);
extern void int3(void);
extern void int4(void);
extern void int5(void);
extern void int6(void);
extern void int7(void);
extern void int8(void);
extern void int9(void);
extern void int10(void);
extern void int11(void);
extern void int12(void);
extern void int13(void);
extern void int14(void);
extern void int15(void);
extern void int16(void);
extern void int17(void);
extern void int18(void);
extern void int19(void);
extern void int20(void);
extern void int21(void);
extern void int22(void);
extern void int23(void);
extern void int24(void);
extern void int25(void);
extern void int26(void);
extern void int27(void);
extern void int28(void);
extern void int29(void);
extern void int30(void);
extern void int31(void);
#endif
