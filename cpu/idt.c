#include <stdint.h>
#include "idt.h"
#include "../common.h"

uint16_t get_low(uint32_t num){ return num & 0xffff; }
uint16_t get_high(uint32_t num){ return (num >> 16) & 0xffff; }

extern idt_flush(u32int);

gate_t idt[256];
idt_register idt_reg;

void set_idt_gate(int index, uint32_t base, uint8_t flags, uint16_t sel)
{
    idt[index].low_offset = get_low(base);
    idt[index].selector = sel;
    idt[index].unused = 0;
    idt[index].flags = flags;
    idt[index].high_offset = get_high(base);
}

void init_idt()
{
    idt_reg.base = (u32int)&idt;
    idt_reg.limit = 256 * sizeof(gate_t) -1; // minus one because 0 indexed


    set_idt_gate(0, (u32int)isr0, 0x8E, 0x08);

    idt_flush((u32int)&idt_reg);
}
