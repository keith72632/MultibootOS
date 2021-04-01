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
 
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
//#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
//#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 

void enable_cursor(u8int cursor_start, u8int cursor_end)
{
	port_byte_out(0x3D4, 0x0A);
	port_byte_out(0x3D5, (port_byte_in((0x3D5)&0xC0) | cursor_start));
	port_byte_out(0x3D4, 0x0B);
	port_byte_out(0x3D5, (port_byte_in((0x3D5) & 0xE0) | cursor_end));
}
 
void kernel_main(void) 
{
	init_descriptor_tables();
	init_timer(50);
	/*Grub disables cursor, So need to initalize first*/
	enable_cursor(14, 15);
	//terminal_initialize();
 
	/* Newline support is left as an exercise. */
	printk("hello bitches\n");

	asm volatile("int $0x3");
	asm volatile("int $0x11");
	asm volatile("int $0x1f");
}
