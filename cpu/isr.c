#include "isr.h"
#include "../drivers/display.h"

void isr_handler(registers_t regs)
{
   printk("\nrecieved interrupt: ");
   print_dec(regs.int_no);
   printk('\n');
}

