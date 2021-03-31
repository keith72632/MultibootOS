/**************************************************************************************
 *                  Interrut Request Servces (IRQs)                                   *
 **************************************************************************************
 In protect mode with GRUB IRQs must be remapped as to not confilct with other processes.
 An IDT (Interrupt desriptor table) must be established. Consists of 256 gates. Each gate
 specifies the 32 bit offset split into half, segement selector, and flags to be set refering
 to the use privilege. The offset is establish by feeding a 'handler' which is then coded in 
 assembly. In our flat model operating system, segment selector is the same. 
 ***************************************************************************************
*/

/***************************************************************************************
 *                    Interrupt Requests (IRQs)                                        *
 ***************************************************************************************
 This is one of two ways of commuincating with external devices (interrupts and polling)
 All devices that are interrupt capable are connected via line to PIC (Progammable Interrupt
 Controller). Modern pcs have to PICs, a master and a slave, a total of 15 interrupt devices.
 Change of interrupt number is possible by witring to ports via the IRQ line. This is used to 
 remap default IRQs because the original no conflict with the IRQs coded for 32 bit. Ports for
 each PIC are: Master = 0x20 for command and 0x21 for data, Slave=0xA0 for command and 0xA1 for 
 data.
*/

#include "idt.h"
#include "../utils/common.h"
#include "ports.h"

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

    /*Remapping of irq table*/
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);

    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);

    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);

    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);

    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0);

    idt_flush((u32int)&idt_reg);
}
