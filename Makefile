CC = i386-elf-gcc
LINK = i386-elf-ld
FLAGS =  -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

CC_SOURCES = $(wildcard *.c cpu/*.c)

all:run

boot.o: boot.asm
	nasm -felf32 $^ -o $@
kernel.o: kernel.c
	i386-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
cpu/ports.o: cpu/ports.c
	i386-elf-gcc -c cpu/ports.c -o cpu/ports.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
os-image.bin: boot.o kernel.o cpu/ports.o
	${CC} -T linker.ld -o os-image.bin -ffreestanding -O2 -nostdlib $^ -lgcc
os-image.iso: os-image.bin
	cp os-image.bin isodir/boot/
	grub-mkrescue -o os-image.iso isodir
run: os-image.iso
	qemu-system-i386 -cdrom $^
%.o: %.c
	${CC} -c ${CC_SOURCES} -o $@ ${FLAGS}
clean:
	rm *.o *.bin *.iso cpu/*.o