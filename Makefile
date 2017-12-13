AS = as
CC = gcc
LD = ld

INCLUDE = -I./
BINDIR = bin
SRCDIR = tinx
BOOTDIR = boot
SRC = $(wildcard $(SRCDIR)/*.c)
ASM = $(wildcard $(SRCDIR)/*.s) $(wildcard $(BOOTDIR)/*.s)

OBJ := $(addprefix $(BINDIR)/,$(notdir $(SRC:.c=.o)))
OBJ += $(addprefix $(BINDIR)/,$(notdir $(ASM:.s=.o)))
LSCRIPT = link.ld

GCFLAGS = -c -g -Os -m32 -ffreestanding -Wall -Werror -fno-pie
GCFLAGS += $(INCLUDE) -fno-stack-protector
ASFLAGS = --32
LDFLAGS = -static -melf_i386 -nostdlib --nmagic --oformat=binary
KERNELO = $(BINDIR)/head.o
KERNELO += $(BINDIR)/kernel.o $(BINDIR)/desc.o $(BINDIR)/descriptor_tables.o

all:clean $(BINDIR)/boot.bin $(BINDIR)/kernel.bin dd test

clean:
	rm -rf $(BINDIR)

$(BINDIR)/kernel.bin: $(OBJ)
	$(LD) -T$(LSCRIPT) $(LDFLAGS) $(KERNELO) -o $(BINDIR)/kernel.bin
	#objcopy -O binary $(BINDIR)/kernel.elf $(BINDIR)/kernel.bin

$(BINDIR)/boot.bin: $(OBJ)
	$(LD) -Ttext 0x7c00 --oformat=binary $(BINDIR)/boot.o -o $(BINDIR)/boot.bin

$(BINDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(GCFLAGS) -c $< -o $@

$(BINDIR)/%.o: $(SRCDIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(BINDIR)/%.o: $(BOOTDIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $< -o $@

dd:
	dd if=./$(BINDIR)/boot.bin of=./floppy.img obs=512 count=1 conv=notrunc
	dd if=./$(BINDIR)/kernel.bin of=./floppy.img obs=512 seek=1 conv=notrunc

test:
	bochs -f bochsrc

floppy:
	dd if=/dev/zero of=./floppy.img bs=512 count=2880
