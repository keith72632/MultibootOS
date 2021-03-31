#include "isr.h"
#include "../drivers/display.h"

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

