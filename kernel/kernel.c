#include <stdbool.h>
#include <stddef.h>
#include "../utils/common.h"
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

	printk("TrashOS\n>");

	enable_interrupts();
	
	init_keyboard();

	/*Loops cpu*/
	cpu_continue();
}

/****************************************************************************
 *                              Kernel Main                                 *
 ****************************************************************************/
 

void enable_interrupts()
{
	asm volatile("sti");
}

void cpu_continue()
{
	for(;;)
		asm volatile("hlt");
}