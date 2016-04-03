ASM = nasm

all: bootlaoder
.s.o:
	$(ASM) $< -o
	

bootlaoder:
	cd ./boot && \
	  $(ASM) boot.s -o boot.bin
	dd if=./boot/boot.bin of=./hdc.img bs=512 count=1 conv=notrunc

bochs:
	bochs -f bochsrc

qemu:
	qemu-system-i386 -fda hdc.img

hdc:
	dd if=/dev/zero of=./hdc.img bs=512 count=2880

com:
	cd ./boot && \
	  $(ASM) boot.s -o a.com
	sudo mount ./floppy.img /mnt/kernel
	sudo cp ./boot/a.com /mnt/kernel
	sudo umount /mnt/kernel

dos:
	qemu-system-i386 -fda freedos.img -fdb floppy.img

debug:
	qemu-system-i386 -S -s -fda freedos.img -fdb floppy.img -boot a &
	sleep 1
	cgdb -x tools/gdbinit
