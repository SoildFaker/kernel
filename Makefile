AS = as
CC = gcc
LD = ld

INCLUDE = -I./include

DISKIMG = ./80m.img
OUTDIR = bin

EXCLUDE = -not -path "./boot/*" -not -path "./tools/*" -not -path "./user/*"
KNL_CSRC = $(shell find . -name "*.c" $(EXCLUDE))
KNL_SSRC = $(shell find . -name "*.s" $(EXCLUDE))

OBJS = $(shell find . -name "*.o")
KNL_COBJ = $(patsubst %.c, %.o, $(KNL_CSRC))
KNL_SOBJ = $(patsubst %.s, %.o, $(KNL_SSRC))

KNL_LD = tools/kernel_link.ld
BTL_LD = tools/loader_link.ld
USR_LD = tools/user_link.ld

GCFLAGS = -c -g -Os -m32 -ffreestanding -Wall -Werror -fno-pie
GCFLAGS += $(INCLUDE) -fno-stack-protector
ASFLAGS = --32
MAPFLAGS = -Map kernel.map
KNL_LDFLAGS = -static -nostdlib --nmagic -melf_i386 
BTL_LDFLAGS = -static -nostdlib --nmagic --oformat=binary -melf_i386 

BTL_OBJ = ./boot/loaderasm.o ./boot/loadermain.o ./drivers/hd.o 
BTL_OBJ += ./kernel/string.o ./kernel/elf.o

all:$(OUTDIR)/boot.bin $(OUTDIR)/loader.bin $(OUTDIR)/kernel.elf sh dd test

sh:
	$(CC) $(GCFLAGS) -c ./user/sh.c -o ./user/sh.o &\
	$(LD) -T$(USR_LD) $(KNL_LDFLAGS) ./user/sh.o -o $(OUTDIR)/sh.elf

clean:
	rm -rf $(OUTDIR)/*
	rm -rf $(OBJS)
	rm -rf kernel.img

$(OUTDIR)/kernel.elf: $(KNL_COBJ) $(KNL_SOBJ)
	$(LD) -T$(KNL_LD) $(KNL_LDFLAGS) $(KNL_COBJ) $(KNL_SOBJ) -o $(OUTDIR)/kernel.elf $(MAPFLAGS)

$(OUTDIR)/loader.bin: $(BTL_OBJ)
	$(LD) -T$(BTL_LD) $(BTL_LDFLAGS) $(BTL_OBJ) -o $(OUTDIR)/loader.bin

$(OUTDIR)/boot.bin: ./boot/boot.o
	$(LD) -Ttext 0x7c00 --oformat=binary ./boot/boot.o -o $(OUTDIR)/boot.bin

mkfs:
	$(CC) $(INCLUDE) -m32 -Os tools/mkfs.c -o mkfs_kernel

$(KNL_COBJ): %.o: %.c
	$(CC) $(GCFLAGS) -c $< -o $@

$(KNL_SOBJ): %.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

./boot/loadermain.o: ./boot/loadermain.c
	$(CC) $(GCFLAGS) -c $< -o $@

./boot/boot.o: ./boot/boot.s
	$(AS) $< -o $@

./boot/loaderasm.sasm.o: ./boot/loaderasm.s
	$(AS) $(ASFLAGS) $< -o $@

dd:
	./mkfs_kernel bin/loader.bin loader.bin bin/kernel.elf kernel.elf bin/sh.elf sh.elf &\
	dd if=./$(OUTDIR)/boot.bin of=$(DISKIMG) obs=512 count=1 conv=notrunc
	dd if=./$(OUTDIR)/loader.bin of=$(DISKIMG) obs=512 seek=1 conv=notrunc
	dd if=./kernel.img of=$(DISKIMG) obs=512 seek=10 conv=notrunc
#	dd if=./$(OUTDIR)/loader.bin of=$(DISKIMG) obs=512 seek=1 conv=notrunc
#	dd if=./$(OUTDIR)/kernel.elf of=$(DISKIMG) obs=512 seek=3 conv=notrunc

test:
	bochs -f bochsrc

floppy:
	dd if=/dev/zero of=./floppy.img bs=512 count=2880
