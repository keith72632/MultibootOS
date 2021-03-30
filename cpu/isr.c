#include "isr.h"
#include "../drivers/display.h"

void isr_handler(registers_t regs)
{
   printk("recieved interrupt: ");
   print_dec(regs.int_no);
   //monitor_put('\n');
}

