AS = as
CC = gcc
LD = ld

INCLUDE = -I./include

DISKIMG = ./80m.img
OUTDIR = bin
KNLDIR = kernel
INITDIR = init
DRIVERDIR = drivers
MMDIR = mm
BTLDIR = boot

KNL_SRC = $(wildcard $(KNLDIR)/*.c) $(wildcard drivers/*.c) 
KNL_SRC += $(wildcard $(MMDIR)/*.c)
KNL_SRC += $(wildcard $(INITDIR)/*.c)
KNL_ASM = $(wildcard $(KNLDIR)/*.s)

KNL_OBJ := $(addprefix $(OUTDIR)/,$(notdir $(KNL_SRC:.c=.o)))
KNL_OBJ += $(addprefix $(OUTDIR)/,$(notdir $(KNL_ASM:.s=.o)))

KNL_LD = tools/kernel_link.ld
BTL_LD = tools/loader_link.ld

GCFLAGS = -c -g -Os -m32 -ffreestanding -Wall -Werror -fno-pie
GCFLAGS += $(INCLUDE) -fno-stack-protector
ASFLAGS = --32
KNL_LDFLAGS = -static -nostdlib --nmagic -melf_i386 
BTL_LDFLAGS = -static -nostdlib --nmagic --oformat=binary -melf_i386 

all:clean $(OUTDIR)/boot.bin $(OUTDIR)/loader.bin $(OUTDIR)/kernel.elf dd test

clean:
	rm -rf $(OUTDIR)

$(OUTDIR)/kernel.elf: $(KNL_OBJ)
	$(LD) -T$(KNL_LD) $(KNL_LDFLAGS) $(KNL_OBJ) -o $(OUTDIR)/kernel.elf

$(OUTDIR)/loader.bin: $(OUTDIR)/loaderasm.o $(OUTDIR)/loadermain.o
	$(LD) -T$(BTL_LD) $(BTL_LDFLAGS) $(OUTDIR)/loaderasm.o $(OUTDIR)/loadermain.o -o $(OUTDIR)/loader.bin

$(OUTDIR)/boot.bin: $(OUTDIR)/boot.o
	$(LD) -Ttext 0x7c00 --oformat=binary $(OUTDIR)/boot.o -o $(OUTDIR)/boot.bin

mkfs:
	$(CC) tools/mkfs.c -o mkfs_kernel

$(OUTDIR)/%.o: $(KNLDIR)/%.c 
	@mkdir -p $(dir $@)
	$(CC) $(GCFLAGS) -c $< -o $@

$(OUTDIR)/%.o: $(MMDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(GCFLAGS) -c $< -o $@

$(OUTDIR)/%.o: $(INITDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(GCFLAGS) -c $< -o $@

$(OUTDIR)/%.o: $(DRIVERDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(GCFLAGS) -c $< -o $@

$(OUTDIR)/%.o: $(KNLDIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(OUTDIR)/loadermain.o: $(BTLDIR)/loadermain.c
	@mkdir -p $(dir $@)
	$(CC) $(GCFLAGS) -c $< -o $@

$(OUTDIR)/boot.o: $(BTLDIR)/boot.s
	@mkdir -p $(dir $@)
	$(AS) $< -o $@

$(OUTDIR)/loaderasm.o: $(BTLDIR)/loaderasm.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

dd:
	dd if=./$(OUTDIR)/boot.bin of=$(DISKIMG) obs=512 count=1 conv=notrunc
	dd if=./$(OUTDIR)/loader.bin of=$(DISKIMG) obs=512 seek=1 conv=notrunc
	dd if=./$(OUTDIR)/kernel.elf of=$(DISKIMG) obs=512 seek=3 conv=notrunc

test:
	bochs -f bochsrc

floppy:
	dd if=/dev/zero of=./floppy.img bs=512 count=2880
