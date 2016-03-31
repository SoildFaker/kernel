ASM = nasm

all: bootlaoder
.s.o:
	$(ASM) $< -o
	

bootlaoder:
	$(ASM) boot/boot.s -o boot/boot.bin
	dd if=./boot/boot.bin of=./hdc.img bs=512 count=1



qemu:
	qemu-system-i386 -fda hdc.img

hdc:
	dd if=/dev/zero of=./hdc.img bs=512 count=2880

dos:
	sudo mount ./hdc.img /mnt/kernel
	sudo cp ./boot/boot.bin /mnt/kernel
	sudo umount /mnt/kernel
	qemu-system-i386 -fda freedos.img -fdb hdc.img
