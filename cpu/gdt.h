#ifndef __GDT_H
#define __GDT_H

/* GDT.C */
extern void gdt_flush(u32int);
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
extern void gdt_install();
void init_descriptor_tables();
#endif
