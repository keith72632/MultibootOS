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
#include "ports.h"

u32int tick = 0;


static void timer_callback(registers_t regs)
{
    tick++;
    printk("Tick: ");
    print_dec(tick);
    printk("\n");
}

void init_timer(u32int frequency)
{
    //registers interrupt for system clock at index 32 of interrupt_handlers
    register_interrupt_handlers(32, &timer_callback);

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   u32int divisor = 1193180 / frequency;

    //send byte to command register 
    //sets the PIT to repeating mode, so when counters divisor reaches zero, its automatically refreshed
    port_byte_out(0x43, 0x36);

    u8int low = (divisor & 0xFF);
    u8int high = (divisor >> 8) & 0xFF;

    //send the frequency drive
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);

}
