CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
LINK = /usr/local/i386elfgcc/bin/i386-elf-ld
FLAGS =  -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

CC_SOURCES = $(wildcard *.c cpu/*.c drivers/*.c)
HEADERS = $(wildcard cpu/*.h drivers/*.h)
OBJ_FILES = ${C_SOURCES:.c=.o}
all:run

boot.o: boot.asm
	nasm -felf32 $^ -o $@
cpu/ports.o: cpu/ports.c
	${CC} -c cpu/ports.c -o cpu/ports.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
drivers/display.o: 
	${CC} -c drivers/display.c -o drivers/display.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
kernel.o: kernel.c
	${CC} -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
os-image.bin: boot.o kernel.o cpu/ports.o drivers/display.o ${HEADERS}
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