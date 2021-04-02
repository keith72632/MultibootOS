#include "isr.h"
#include "../drivers/display.h"
#include "timer.h"

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

/*called in interrupt.asm*/
void isr_handler(registers_t regs)
{
   printk("\nrecieved interrupt: ");
   print_dec(regs.int_no);
   printk(exception_messages[regs.int_no - 1]);
   printk('\n');
}

/*This is a custom type array of all handlers*/
isr_t irq_interrupt_handlers[256];

/*This will be called in every driver function to register handler to irq_interrupt_handlers[]*/
void register_interrupt_handlers(u8int n, isr_t handler)
{
	irq_interrupt_handlers[n] = handler;
}

/*Must tell processor when finished with IRQ*/
void irq_handler(registers_t regs)
{

	if(irq_interrupt_handlers[regs.int_no] != 0)
	{
		/*this is a custom function handler that allow for custum interrupts. It takes the errors and numbers from 
		*registers in asm, ad adds this interrupt to an isr_t array in the indexed postion that is same as the int_no
		*in interrupt*/
		isr_t handler = irq_interrupt_handlers[regs.int_no];
		handler(regs);
	}	
    //Send EOI signal to PICs
    //Check if Slave 
    if(regs.int_no >= 40){
        //Send reset to slave
        outb(0xA0, 0x20);
    } else {
    	//Send reset to master
    	outb(0x20, 0x20);
    }

}

