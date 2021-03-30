#include <stdint.h>

typedef struct {
    uint16_t low_offset;          //first 16
    uint16_t selector;            //has to be 0 so the iret instruction won't throw a #GP exception
    uint8_t unused;               //always zero
    uint8_t type: 4;
    uint8_t storage_segment: 1;
    uint8_t description_priv_level: 2;
    uint8_t present: 1;
    uint16_t high_offset;
}__attribute__((packed)) gate_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
}__attribute__((packed)) idt_register;

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();