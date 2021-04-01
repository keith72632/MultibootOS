#include "../utils/common.h"
#include "../cpu/isr.h"
#include "../cpu/ports.h"
#include "display.h"

void keyboard_callback()
{
    u8int scancode = port_byte_in(0x60);
    print_dec(scancode);
}

void init_keyboard()
{
    register_interrupt_handlers(IRQ1, keyboard_callback);
}