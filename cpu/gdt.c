/*
 ************************************************************************************
 *Global Descriptor Table
 *x86 has two methods of memory protection and providing virtual memory: segmentation and paging
 *Segmentation-every memory access is evalutated with respect to the segment
 *Paging-every addresss space is split into blocks called pages
 *Segmentation is slowly becoming obsolete.x86-64 requires flat mem model(base 0 - 0xffffffffff)
 *Segmentation allows for the setting of ring levels*/

#include "../utils/common.h"
#include "gdt.h"
#include "idt.h"
#include "../utils/common.h"
#include "isr.h"

/* Defines a GDT entry.  We say packed, because it prevents the
 * compiler from doing things that it thinks is best, i.e.
 * optimization, etc. */
struct gdt_entry_struct
{
	u16int limit_low;       //lower 16 bits of limit
	u16int base_low;        //lower 16 bits of base
	u8int base_middle;      //next 8 bits of base
	u8int access;           //access flags, determines ring priv. 
    //access flag layout: [gran|d|0|A|type|descriptor type|descript level priv|is seg present]
	u8int granularity;      
	u8int base_high;        //last 8 bits of base
} __attribute__((packed));

/* Special pointer which includes the limit: The max bytes
 * taken up by the GDT, minus 1.  Again, this NEEDS to be
 * packed */
struct gdt_ptr_struct
{
	u16int limit;          //upper 16 bits of all selector limits
	u32int base;             //address of first gdt_entry_t struct 
} __attribute__((packed));

/* Type definitions */
typedef unsigned long int addr;
typedef struct gdt_entry_struct gdt_entry_t;
typedef struct gdt_ptr_struct gdt_ptr_t;



/* Our GDT, with 3 entries, and finally our special GDT pointer */
gdt_entry_t gdt[5];
gdt_ptr_t gdt_ptr;


/* This will be a function in start.asm.  We use this to properly
 * reload the new segment registers */
//extern void _gdt_flush();

/* Setup a descriptor in the Global Descriptor Table */
void gdt_set_gate(s32int num, u32int base, u32int limit, u8int access, u8int gran)
{
	/* Setup the descriptor base access */
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;

	/* Setup the descriptor limits */
	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = ((limit >> 16) & 0x0F);

	/* Finally, set up the granularity and access flags */
	gdt[num].granularity |= gran & 0xF0;
	gdt[num].access = access;
}

/* Should be called by main.  This will setup the special GDT
 * pointer, set up the 6 entries in our GDT, and then finally
 * call gdt_flush() in our assembler file in order to tell
 * the processor where the new GDT is and update the new segment
 * registers. */
void gdt_install()
{
	/* Setup the GDT pointer and limit */
	gdt_ptr.limit = (sizeof(struct gdt_entry_struct) * 5) - 1;
	gdt_ptr.base = (addr)&gdt;            //pointer to address of gdt[5]

	/*Notice the only thing that changes in each gate in the access byte, which determines user priviledge*/
	/* Our NULL descriptor */
	gdt_set_gate(0, 0, 0, 0, 0);

	/* The second entry is our Code Segment.  The base address
	 * is 0, the limit is 4 gigabytes, it uses 4 kilobyte
	 * granularity, uses 32-bit opcodes, and is a Code Segment
	 * descriptor.  Please check the table above in the tutorial
	 * in order to see exactly what each value means */
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);//flags to 0b10011010

	/* The third entry is our Data Segment.  It's exactly the
	 * same as our code segment, but the descriptor type in
	 * this entry's access byte says it's a Data Segment */
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);//flags to 0b10010010

	/* Install the user mode segments into the GDT */
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);//flags to 0b11111010
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);//flags to 0b11110010

	/* Install the TSS into the GDT */
	//tss_install(5, 0x10, 0x0);

	/* Flush our the old GDT / TSS and install the new changes! */
	gdt_flush((u32int)&gdt_ptr);
}

void init_descriptor_tables()
{
	gdt_install();
	init_idt();

	//nullify al interrupt handlers
	memset(&irq_interrupt_handlers, 0, sizeof(isr_t)*256);
}
