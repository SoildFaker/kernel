ASM = as

all: bootlaoder
.s.o:
	$(ASM) $< -o

bootlaoder:
	cd ./boot && \
	  $(ASM) boot.s -o boot.bin
	dd if=./boot/boot.bin of=./floppy.img bs=512 count=1 conv=notrunc

bochs:
	bochs -f bochsrc

floppy:
	dd if=/dev/zero of=./floppy.img bs=512 count=2880
