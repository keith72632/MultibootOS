#include "idt.h"
#include "../utils/common.h"

u16int get_low(u32int num){ return num & 0xffff; }
u16int get_high(u32int num){ return (num >> 16) & 0xffff; }

extern idt_flush(u32int);

gate_t idt[256];
idt_register idt_reg;

void set_idt_gate(int index, u32int base, u8int flags, u16int sel)
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
    set_idt_gate(1, (u32int)isr1, 0x8E, 0x08);
    set_idt_gate(2, (u32int)isr2, 0x8E, 0x08);
    set_idt_gate(3, (u32int)isr3, 0x8E, 0x08);
    set_idt_gate(4, (u32int)isr4, 0x8E, 0x08);
    set_idt_gate(5, (u32int)isr5, 0x8E, 0x08);
    set_idt_gate(6, (u32int)isr6, 0x8E, 0x08);
    set_idt_gate(7, (u32int)isr7, 0x8E, 0x08);
    set_idt_gate(8, (u32int)isr8, 0x8E, 0x08);
    set_idt_gate(9, (u32int)isr9, 0x8E, 0x08);
    set_idt_gate(10, (u32int)isr10, 0x8E, 0x08);
    set_idt_gate(11, (u32int)isr11, 0x8E, 0x08);
    set_idt_gate(12, (u32int)isr12, 0x8E, 0x08);
    set_idt_gate(13, (u32int)isr13, 0x8E, 0x08);
    set_idt_gate(14, (u32int)isr14, 0x8E, 0x08);
    set_idt_gate(15, (u32int)isr15, 0x8E, 0x08);
    set_idt_gate(16, (u32int)isr16, 0x8E, 0x08);
    set_idt_gate(17, (u32int)isr17, 0x8E, 0x08);
    set_idt_gate(18, (u32int)isr18, 0x8E, 0x08);
    set_idt_gate(19, (u32int)isr19, 0x8E, 0x08);
    set_idt_gate(20, (u32int)isr20, 0x8E, 0x08);
    set_idt_gate(21, (u32int)isr21, 0x8E, 0x08);
    set_idt_gate(22, (u32int)isr22, 0x8E, 0x08);
    set_idt_gate(23, (u32int)isr23, 0x8E, 0x08);
    set_idt_gate(24, (u32int)isr24, 0x8E, 0x08);
    set_idt_gate(25, (u32int)isr25, 0x8E, 0x08);
    set_idt_gate(26, (u32int)isr26, 0x8E, 0x08);
    set_idt_gate(27, (u32int)isr27, 0x8E, 0x08);
    set_idt_gate(28, (u32int)isr28, 0x8E, 0x08);
    set_idt_gate(29, (u32int)isr29, 0x8E, 0x08);
    set_idt_gate(30, (u32int)isr30, 0x8E, 0x08);
    set_idt_gate(31, (u32int)isr31, 0x8E, 0x08);

    idt_flush((u32int)&idt_reg);
}
