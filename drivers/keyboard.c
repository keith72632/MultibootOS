#include "../utils/common.h"
#include "../cpu/isr.h"
#include "../cpu/ports.h"
#include "display.h"
#include "../cpu/timer.h"\

#define ENTER 0x1C
#define TAB 0x0F


u8int scancodes[] = {
    '?', '?','1', '2', '3', 
    '4', '5', '6', '7', '8',
    '9', '0', '?', '?', '?', 
    '?', 'q', 'w', 'e', 'r', 
    't', 'y', 'u', 'i', 'o', 
    'p', '[', ']', '?', 'a',
    's', 'd', 'f', 'g', 'h',
    'j', 'k', 'l', 'z', 'x', 
    'c', 'v', 'b', 'n', 'm' 
};

void keyboard_callback()
{
    u8int scancode = port_byte_in(0x60);
    int offset = get_cursor();
    if(scancode > 100) return;
    //printc(scancodes[scancode]);
    print_dec(scancode);
}

void init_keyboard()
{
    init_timer(50);
    register_interrupt_handlers(IRQ1, keyboard_callback);
}