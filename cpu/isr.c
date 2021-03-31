#include "isr.h"
#include "../drivers/display.h"
#include "ports.h"

/* To print the message which defines every exception */
char *exception_messages[] = {
        ": Division By Zero",
        ": Debug",
        ": Non Maskable Interrupt",
        ": Breakpoint",
        ": Into Detected Overflow",
        ": Out of Bounds",
        ": Invalid Opcode",
        ": No Coprocessor",

        ": Double Fault",
        ": Coprocessor Segment Overrun",
        ": Bad TSS",
        ": Segment Not Present",
        ": Stack Fault",
        ": General Protection Fault",
        ": Page Fault",
        ": Unknown Interrupt",

        ": Coprocessor Fault",
        ": Alignment Check",
        ": Machine Check",
        ": Reserved",
        ": Reserved",
        ": Reserved",
        ": Reserved",
        ": Reserved",

        ": Reserved",
        ": Reserved",
        ": Reserved",
        ": Reserved",
        ": Reserved",
        ": Reserved",
        ": Reserved",
        ": Reserved"
};

void isr_handler(registers_t regs)
{
   printk("\nrecieved interrupt: ");
   print_dec(regs.int_no);
   printk(exception_messages[regs.int_no - 1]);
   printk('\n');
}

isr_t interrupt_handlers[256];

void register_interrupt_handlers(u8int n, isr_t handler)
{
	interrupt_handlers[n] = handler;
}

/*Must tell processor when finished with IRQ*/
void irq_handler(registers_t regs)
{
    //Send EOI signal to PICs
    //Check if Slave 
    if(regs.int_no >= 40){
        //Send reset to slave
        port_byte_out(0xA0, 0x20);
    }
	//Send reset to master
	port_byte_out(0x20, 0x20);

	if(interrupt_handlers[regs.int_no] != 0)
	{
		/*this is a custom function handler that allow for custum interrupts. It takes the errors and numbers from 
		*registers in asm, ad adds this interrupt to an isr_t array in the indexed postion that is same as the int_no
		*in interrupt*/
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}	
}

