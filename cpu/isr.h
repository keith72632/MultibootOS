//
// isr.h -- Interface and structures for high level interrupt service routines.
// Part of this code is modified from Bran's kernel development tutorials.
// Rewritten for JamesM's kernel development tutorials.
//

#include "../utils/common.h"


typedef struct registers
{
   u32int ds;                  // Data segment selector
   u32int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   u32int int_no, err_code;    // Interrupt number and error code (if applicable)
   u32int eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t; 

/*this is a custom function handler that allow for custum interrupts. It takes the errors and numbers from 
*registers in asm, ad adds this interrupt to an isr_t array in the indexed postion that is same as the int_no
*in interrupt*/
typedef void (*isr_t)(registers_t);
void register_interrupt_handler(u8int n, isr_t handler);
