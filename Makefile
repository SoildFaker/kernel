AS = as
CC = gcc
LD = ld

INCLUDE = -I./include

OUTDIR = bin
KNLDIR = kernel
BTLDDIR = boot

KNLSRC = $(wildcard $(KNLDIR)/*.c)
KNLASM = $(wildcard $(KNLDIR)/*.s)
# We need head.o be placed ahead, so filter out here
KNLASM := $(filter-out head.s, $(KNLASM))
BTLDASM = $(wildcard $(BTLDDIR)/*.s)

KNLOBJ := $(addprefix $(OUTDIR)/,$(notdir $(KNLSRC:.c=.o)))
KNLOBJ += $(addprefix $(OUTDIR)/,$(notdir $(KNLASM:.s=.o)))
BTLDOBJ = $(addprefix $(OUTDIR)/,$(notdir $(BTLDASM:.s=.o)))

LSCRIPT = link.ld

GCFLAGS = -c -g -Os -m32 -ffreestanding -Wall -Werror -fno-pie
GCFLAGS += $(INCLUDE) -fno-stack-protector
ASFLAGS = --32
LDFLAGS = -static -melf_i386 -nostdlib --nmagic --oformat=binary

all:clean $(OUTDIR)/boot.bin $(OUTDIR)/kernel.bin dd test

clean:
	rm -rf $(OUTDIR)

$(OUTDIR)/kernel.bin: $(KNLOBJ)
	$(LD) -T$(LSCRIPT) $(LDFLAGS) $(OUTDIR)/head.o $(KNLOBJ) -o $(OUTDIR)/kernel.bin
	#objcopy -O binary $(OUTDIR)/kernel.elf $(OUTDIR)/kernel.bin

$(OUTDIR)/boot.bin: $(BTLDOBJ)
	$(LD) -Ttext 0x7c00 --oformat=binary $(BTLDOBJ) -o $(OUTDIR)/boot.bin

$(OUTDIR)/%.o: $(KNLDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(GCFLAGS) -c $< -o $@

$(OUTDIR)/%.o: $(KNLDIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(OUTDIR)/%.o: $(BTLDDIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $< -o $@

dd:
	dd if=./$(OUTDIR)/boot.bin of=./floppy.img obs=512 count=1 conv=notrunc
	dd if=./$(OUTDIR)/kernel.bin of=./floppy.img obs=512 seek=1 conv=notrunc

test:
	bochs -f bochsrc

floppy:
	dd if=/dev/zero of=./floppy.img bs=512 count=2880
