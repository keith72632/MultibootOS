/*****************************************************************************************
 *                           The PIT(system clock)                                       *
 *****************************************************************************************
The programmable interval timer is a chip connect to IRQ0. It can interrupt the system at
a user defined rate (18.2hz and 1.1931mhz). Clock is fed through frequency driver, set to 
modulate output frequency. Channel 0 is the one used here. PIT needs to interrupt at regular 
interval f. f is generally set to 100Hz(once every 10 milliseconds). To do this, send PIT 
a divisor.
*/

#include "timer.h"
#include "isr.h"
#include "../drivers/display.h"
#include "../utils/common.h"

u32int tick = 0;

static void timer_callback(registers_t regs)
{
  
    tick++;

}

void init_timer(u32int frequency)
{
    // Firstly, register our timer callback.
    register_interrupt_handlers(IRQ0, &timer_callback);

    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    u32int divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    u8int l = (u8int)(divisor & 0xFF);
    u8int h = (u8int)( (divisor>>8) & 0xFF );

    // Send the frequency divisor.
    outb(0x40, l);
    outb(0x40, h);
}

