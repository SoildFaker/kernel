AS = as
CC = gcc
LD = ld

INCLUDE = -I./include

OUTDIR = bin
KNLDIR = kernel
DRIVERDIR = drivers
MMDIR = mm
BTLDDIR = boot

KNLSRC = $(wildcard $(KNLDIR)/*.c) $(wildcard drivers/*.c) 
KNLSRC += $(wildcard $(MMDIR)/*.c)

KNLASM = $(wildcard $(KNLDIR)/*.s)
# We need head.o be placed ahead, so filter out here
KNLASM := $(filter-out head.s, $(KNLASM))
#KNLSRC := $(filter-out kernel.c, $(KNLSRC))
BTLDASM = $(wildcard $(BTLDDIR)/*.s)

KNLOBJ := $(addprefix $(OUTDIR)/,$(notdir $(KNLSRC:.c=.o)))
KNLOBJ += $(addprefix $(OUTDIR)/,$(notdir $(KNLASM:.s=.o)))
BTLDOBJ = $(addprefix $(OUTDIR)/,$(notdir $(BTLDASM:.s=.o)))

LSCRIPT = link.ld

GCFLAGS = -c -g -Os -m32 -ffreestanding -Wall -Werror -fno-pie
GCFLAGS += $(INCLUDE) -fno-stack-protector
ASFLAGS = --32
LDFLAGS = -static -nostdlib --nmagic --oformat=binary -melf_i386 

all:clean $(OUTDIR)/boot.bin $(OUTDIR)/loader.bin $(OUTDIR)/kernel.bin dd test

clean:
	rm -rf $(OUTDIR)

$(OUTDIR)/kernel.bin: $(KNLOBJ)
	$(LD) -T$(LSCRIPT) $(LDFLAGS) $(OUTDIR)/head.o $(KNLOBJ) -o $(OUTDIR)/kernel.bin
	#objcopy -O binary $(OUTDIR)/kernel.elf $(OUTDIR)/kernel.bin

$(OUTDIR)/boot.bin: $(BTLDOBJ)
	$(LD) -Ttext 0x7c00 --oformat=binary $(OUTDIR)/boot.o -o $(OUTDIR)/boot.bin

$(OUTDIR)/loader.bin: $(BTLDOBJ)
	$(LD) -Ttext 0x8000 $(LDFLAGS) $(OUTDIR)/loader.o -o $(OUTDIR)/loader.bin

$(OUTDIR)/%.o: $(KNLDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(GCFLAGS) -c $< -o $@

$(OUTDIR)/%.o: $(MMDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(GCFLAGS) -c $< -o $@

$(OUTDIR)/%.o: $(DRIVERDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(GCFLAGS) -c $< -o $@

$(OUTDIR)/%.o: $(KNLDIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(OUTDIR)/loader.o: $(BTLDDIR)/loader.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(OUTDIR)/boot.o: $(BTLDDIR)/boot.s
	@mkdir -p $(dir $@)
	$(AS) $< -o $@

dd:
	dd if=./$(OUTDIR)/boot.bin of=./floppy.img obs=512 count=1 conv=notrunc
	dd if=./$(OUTDIR)/loader.bin of=./floppy.img obs=512 seek=1 count=1 conv=notrunc
	dd if=./$(OUTDIR)/kernel.bin of=./floppy.img obs=512 seek=2 conv=notrunc

test:
	bochs -f bochsrc

floppy:
	dd if=/dev/zero of=./floppy.img bs=512 count=2880
