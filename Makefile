CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
LINK = /usr/local/i386elfgcc/bin/i386-elf-ld
FLAGS =  -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

CC_SOURCES = $(wildcard *.c cpu/*.c drivers/*.c)
HEADERS = $(wildcard *.h cpu/*.h drivers/*.h)
OBJ_FILES = ${C_SOURCES:.c=.o}
all:run

boot.o: boot.asm
	nasm -felf32 $^ -o $@
gdt.o: gdt.asm
	nasm -felf32 $^ -o $@
interrupts.o: interrupts.asm
	nasm -felf32 $^ -o $@
cpu/ports.o: cpu/ports.c
	${CC} -c cpu/ports.c -o cpu/ports.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
drivers/display.o: 
	${CC} -c drivers/display.c -o drivers/display.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
kernel.o: kernel.c
	${CC} -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
cpu/idt.o: cpu/idt.c
	${CC} -c cpu/idt.c -o cpu/idt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
cpu/isr.o: cpu/isr.c
	${CC} -c cpu/isr.c -o cpu/isr.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
cpu/gdt.o: cpu/gdt.c
	${CC} -c cpu/gdt.c -o cpu/gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
os-image.bin: boot.o gdt.o interrupts.o kernel.o cpu/ports.o drivers/display.o cpu/idt.o cpu/isr.o cpu/gdt.o  ${HEADERS}
	${CC} -T linker.ld -o os-image.bin -ffreestanding -O2 -nostdlib $^ -lgcc
os-image.iso: os-image.bin
	cp os-image.bin isodir/boot/
	grub-mkrescue -o os-image.iso isodir
run: os-image.iso
	qemu-system-i386 -cdrom $^
%.o: %.c
	${CC} ${CC_SOURCES} -o $@ ${FLAGS}
clean:
	rm *.o *.bin *.iso cpu/*.o drivers/*.o isodir/boot/*.bin
