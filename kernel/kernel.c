#include <stdbool.h>
#include <stddef.h>
#include "../utils/common.h"
#include "../cpu/ports.h"
#include "../drivers/display.h"
#include "kernel.h"
#include "../cpu/gdt.h"
#include "../utils/common.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
//#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
//#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 
/****************************************************************************
 *                              Kernel Main                                 *
 ****************************************************************************/

void kernel_main(void) 
{
	init_descriptor_tables();

	printk("TrashOS\n");

	enable_interrupts();
	
	init_keyboard();

	/*Loops cpu*/
	cpu_continue();
}

/****************************************************************************
 *                              Kernel Main                                 *
 ****************************************************************************/

void enable_cursor(u8int cursor_start, u8int cursor_end)
{
	port_byte_out(0x3D4, 0x0A);
	port_byte_out(0x3D5, (port_byte_in((0x3D5)&0xC0) | cursor_start));
	port_byte_out(0x3D4, 0x0B);
	port_byte_out(0x3D5, (port_byte_in((0x3D5) & 0xE0) | cursor_end));
}
 
void enable_interrupts()
{
	asm volatile("sti");
}

void cpu_continue()
{
	for(;;)
		asm volatile("hlt");
}