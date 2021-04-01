#include "../utils/common.h"
#include "../cpu/isr.h"
#include "../cpu/ports.h"
#include "display.h"
#include "../cpu/timer.h"

char scancodes[] = {
    '0', '1', '2', '3', '4', '5', 
    '6', '7', '8', '9', 
};

void keyboard_callback()
{
    u8int scancode = port_byte_in(0x60);
    print_dec(scancode);
}

void init_keyboard()
{
    init_timer(50);
    register_interrupt_handlers(IRQ1, keyboard_callback);
}