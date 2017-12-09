ASM = as

all: cbootloader dd test

.s.o:
	$(ASM) $< -o

bootloader:
	cd ./boot && \
	$(ASM) boot.s -o boot.o && \
	ld -Ttext 0x7c00 --oformat=binary boot.o -o boot.bin

dd:
	dd if=./boot/boot.bin of=./floppy.img bs=512 count=1 conv=notrunc

cbootloader:
	cd ./boot && \
	gcc -c -g -Os -m16 -ffreestanding -Wall -Werror boot.c -o boot.o && \
	ld -static -Tboot.ld -melf_i386 -nostdlib --nmagic -o boot.elf boot.o && \
	objcopy -O binary boot.elf boot.bin

test:
	bochs -f bochsrc

floppy:
	dd if=/dev/zero of=./floppy.img bs=512 count=2880
