#ifndef __GDT_H
#define __GDT_H

/* GDT.C */
#include "../utils/common.h"
extern void gdt_flush(u32int);
extern void gdt_set_gate(s32int num, u32int base, u32int limit, u8int access, u8int gran);
extern void gdt_install();
void init_descriptor_tables();
#endif
