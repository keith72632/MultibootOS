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