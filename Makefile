CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
LINK = /usr/local/i386elfgcc/bin/i386-elf-ld
FLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra

CC_SOURCES = $(wildcard *.c cpu/*.c drivers/*.c utils/*.c)
HEADERS = $(wildcard *.h cpu/*.h drivers/*.h kernel/*.h utils/*.h)
OBJ_FILES = ${C_SOURCES:.c=.o cpu/interrupts.o}
CC_FLAGS = -g -m32 -ffreestanding -c -std=gnu99
all:run

boot.o: boot.asm
	nasm -felf32 $^ -o $@
gdt.o: cpu/gdt.asm
	nasm -felf32 $^ -o $@
idt_flush.o: cpu/idt_flush.asm
	nasm -felf32 $^ -o $@
interrupts.o: cpu/interrupts.asm
	nasm -felf32 $^ -o $@
drivers/display.o: 
	${CC} -c drivers/display.c -o drivers/display.o -std=gnu99 -ffreestanding -O2 -m32
common.o: utils/common.c
	${CC} -c $^ -o $@ ${FLAGS}
timer.o: cpu/timer.c
	${CC} -c $^ -o $@ ${FLAGS}
kernel.o: kernel/kernel.c
	${CC} -c kernel/kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -m32
cpu/idt.o: cpu/idt.c
	${CC} -c cpu/idt.c -o cpu/idt.o -std=gnu99 -ffreestanding -O2 -m32 
isr.o: cpu/isr.c
	${CC} -c cpu/isr.c -o cpu/isr.o -std=gnu99 -ffreestanding -O2 -m32 
cpu/gdt.o: cpu/gdt.c
	${CC} -c cpu/gdt.c -o cpu/gdt.o -std=gnu99 -ffreestanding -O2 -m32 
keyboard.o: drivers/keyboard.c
	${CC} -c $^ -o $@ ${FLAGS}
shell_funcs.o: utils/shell_funcs.c
	${CC} -c $^ -o $@ ${FLAGS}
os-image.bin: boot.o gdt.o idt_flush.o interrupts.o kernel.o drivers/display.o cpu/idt.o cpu/gdt.o cpu/isr.o timer.o common.o keyboard.o shell_funcs.o ${HEADERS}
	${CC} -T linker.ld -o os-image.bin -ffreestanding -O2 -m32 -nostdlib $^ -lgcc
os-image.iso: os-image.bin
	cp os-image.bin isodir/boot/
	grub-mkrescue -o os-image.iso isodir
run: os-image.iso
	qemu-system-i386 -cdrom $^
%.o: %.c ${HEADERS}
	${CC} ${CC_FLAGS} $< -o $@
%.o: %.asm	
	nasm -felf32 $< -o $@ 
clean:
	rm *.o *.bin *.iso cpu/*.o drivers/*.o isodir/boot/*.bin
