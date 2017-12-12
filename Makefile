AS = as
CC = gcc
LD = ld

INCLUDE = -I./
BINDIR = bin
SRCDIR = boot
SRC = $(wildcard $(SRCDIR)/*.c)
ASM = $(wildcard $(SRCDIR)/*.s)

OBJ := $(addprefix $(BINDIR)/,$(notdir $(SRC:.c=.o)))
OBJ += $(addprefix $(BINDIR)/,$(notdir $(ASM:.s=.o)))
LSCRIPT = link.ld

GCFLAGS = -c -g -Os -m16 -ffreestanding -Wall -Werror
GCFLAGS += $(INCLUDE) -fno-stack-protector
ASFLAGS = 
LDFLAGS = -static -T$(LSCRIPT) -melf_i386 -nostdlib --nmagic

all:$(BINDIR)/boot.bin $(BINDIR)/kernel.bin dd test

clean:
	rm -rf $(BINDIR)

$(BINDIR)/kernel.bin: $(OBJ)
	$(LD) $(LDFLAGS) -o $(BINDIR)/kernel.elf $(BINDIR)/head.o && \
	objcopy -O binary $(BINDIR)/kernel.elf $(BINDIR)/kernel.bin

$(BINDIR)/boot.bin: $(OBJ)
	$(LD) -Ttext 0x7c00 --oformat=binary $(BINDIR)/boot.o -o $(BINDIR)/boot.bin

$(BINDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(GCFLAGS) -c $< -o $@

$(BINDIR)/%.o: $(SRCDIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $< -o $@

dd:
	dd if=./$(BINDIR)/boot.bin of=./floppy.img obs=512 count=1 conv=notrunc
	dd if=./$(BINDIR)/kernel.bin of=./floppy.img obs=512 seek=1 conv=notrunc

test:
	bochs -f bochsrc

floppy:
	dd if=/dev/zero of=./floppy.img bs=512 count=2880
