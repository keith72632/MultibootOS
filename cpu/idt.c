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

u16int get_low(u32int num){ return num & 0xffff; }
u16int get_high(u32int num){ return (num >> 16) & 0xffff; }

extern idt_flush(u32int);

gate_t idt[256];
idt_register idt_reg;

void set_idt_gate(u8int index, u32int base, u16int sel, u8int flags)
{
    idt[index].low_offset = base & 0xFFFF;
    idt[index].selector = sel;
    idt[index].unused = 0;
    idt[index].flags = flags;
    idt[index].high_offset = (base >> 16) & 0xFFFF;
}

void init_idt()
{
    idt_reg.base = (u32int)&idt;
    idt_reg.limit = 256 * sizeof(gate_t) -1; // minus one because 0 indexed

    memset(&idt, 0, sizeof(gate_t)*256);
/*argument: interrupt number, handler function in asm, flags, segment offset*/
/*Test internal interrupt by using asm volatile(int $int_no in hex)*/
    set_idt_gate(0, (u32int)isr0, 0x08, 0x8E);
    set_idt_gate(1, (u32int)isr1, 0x08, 0x8E);
    set_idt_gate(2, (u32int)isr2, 0x08, 0x8E);
    set_idt_gate(3, (u32int)isr3, 0x08, 0x8E);
    set_idt_gate(4, (u32int)isr4, 0x08, 0x8E);
    set_idt_gate(5, (u32int)isr5, 0x08, 0x8E);
    set_idt_gate(6, (u32int)isr6, 0x08, 0x8E);
    set_idt_gate(7, (u32int)isr7, 0x08, 0x8E);
    set_idt_gate(8, (u32int)isr8, 0x08, 0x8E);
    set_idt_gate(9, (u32int)isr9, 0x08, 0x8E);
    set_idt_gate(10, (u32int)isr10, 0x08, 0x8E);
    set_idt_gate(11, (u32int)isr11, 0x08, 0x8E);
    set_idt_gate(12, (u32int)isr12, 0x08, 0x8E);
    set_idt_gate(13, (u32int)isr13, 0x08, 0x8E);
    set_idt_gate(14, (u32int)isr14, 0x08, 0x8E);
    set_idt_gate(15, (u32int)isr15, 0x08, 0x8E);
    set_idt_gate(16, (u32int)isr16, 0x08, 0x8E);
    set_idt_gate(17, (u32int)isr17, 0x08, 0x8E);
    set_idt_gate(18, (u32int)isr18, 0x08, 0x8E);
    set_idt_gate(19, (u32int)isr19, 0x08, 0x8E);
    set_idt_gate(20, (u32int)isr20, 0x08, 0x8E);
    set_idt_gate(21, (u32int)isr21, 0x08, 0x8E);
    set_idt_gate(22, (u32int)isr22, 0x08, 0x8E);
    set_idt_gate(23, (u32int)isr23, 0x08, 0x8E);
    set_idt_gate(24, (u32int)isr24, 0x08, 0x8E);
    set_idt_gate(25, (u32int)isr25, 0x08, 0x8E);
    set_idt_gate(26, (u32int)isr26, 0x08, 0x8E);
    set_idt_gate(27, (u32int)isr27, 0x08, 0x8E);
    set_idt_gate(28, (u32int)isr28, 0x08, 0x8E);
    set_idt_gate(29, (u32int)isr29, 0x08, 0x8E);
    set_idt_gate(30, (u32int)isr30, 0x08, 0x8E);
    set_idt_gate(31, (u32int)isr31, 0x08, 0x8E);

    /*Remapping of irq table*/
    /*ICW1(Tells PIC to wait for three input words*/
    outb(0x20, 0x11);        //starts intialization in cascade mode
    outb(0xA0, 0x11);

    /*ICW2(vector offset)*/
    outb(0x21, 0x20);       //ICW2 Master Offset
    outb(0xA1, 0x28);       //ICW2 Slave Offset

    /*ICW3 (Tells PIC how its wired to master/slave*/
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    /*ICW4 (Gives info about environment*/
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    /*OCW1 (operational command word) enable all IRQs*/
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    set_idt_gate(32, (u32int)irq0, 0x08, 0x8E);
    set_idt_gate(33, (u32int)irq1, 0x08, 0x8E);
    set_idt_gate(34, (u32int)irq2, 0x08, 0x8E);
    set_idt_gate(35, (u32int)irq3, 0x08, 0x8E);
    set_idt_gate(36, (u32int)irq4, 0x08, 0x8E);
    set_idt_gate(37, (u32int)irq5, 0x08, 0x8E);
    set_idt_gate(38, (u32int)irq6, 0x08, 0x8E);
    set_idt_gate(39, (u32int)irq7, 0x08, 0x8E);
    set_idt_gate(40, (u32int)irq8, 0x08, 0x8E);
    set_idt_gate(41, (u32int)irq9, 0x08, 0x8E);
    set_idt_gate(42, (u32int)irq10, 0x08, 0x8E);
    set_idt_gate(43, (u32int)irq11, 0x08, 0x8E);
    set_idt_gate(44, (u32int)irq12, 0x08, 0x8E);
    set_idt_gate(45, (u32int)irq13, 0x08, 0x8E);
    set_idt_gate(46, (u32int)irq14, 0x08, 0x8E);
    set_idt_gate(47, (u32int)irq15, 0x08, 0x8E);
    
    /*mov value of stack pointer +4 to eax, then lidt, then return*/
    idt_flush((u32int)&idt_reg);
}
