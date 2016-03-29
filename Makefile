ASM = nasm

all: bootlaoder
.s.o:
	$(ASM) $< -o
	

bootlaoder:
	$(ASM) ./boot/boot.s -o ./boot/boot.bin
	dd if=./boot/boot.bin of=./hdc.img bs=512 count=1



qemu:
	qemu-system-i386 -fda hdc.img
