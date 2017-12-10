ASM = as
CC = gcc
LD = ld

INCLUDE = -I./
BINDIR = bin
SRCDIR = boot
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ := $(addprefix $(BINDIR)/,$(notdir $(SRC:.c=.o)))
LSCRIPT = boot/boot.ld

GCFLAGS = -c -g -Os -m16 -ffreestanding -Wall -Werror 
GCFLAGS += $(INCLUDE) -fno-stack-protector
LDFLAGS = -static -T$(LSCRIPT) -melf_i386 -nostdlib --nmagic

all:$(BINDIR)/boot.bin dd test

clean:
	rm -rf $(BINDIR)

$(BINDIR)/boot.bin: $(BINDIR)/boot.elf
	objcopy -O binary $(BINDIR)/boot.elf $(BINDIR)/boot.bin

$(BINDIR)/boot.elf: $(OBJ)
	$(LD) $(LDFLAGS) -o $(BINDIR)/boot.elf $(OBJ)

$(BINDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(GCFLAGS) -c $< -o $@

bootloader:
	cd ./boot && \
	$(ASM) boot.s -o boot.o && \
	ld -Ttext 0x7c00 --oformat=binary boot.o -o boot.bin

dd:
	dd if=./$(BINDIR)/boot.bin of=./floppy.img bs=512 count=1 conv=notrunc

cbootloader:
	cd ./boot && \
	gcc -c -g -Os -m16 -ffreestanding -Wall -Werror boot.c -o boot.o && \
	ld -static -Tboot.ld -melf_i386 -nostdlib --nmagic -o boot.elf boot.o && \
	objcopy -O binary boot.elf boot.bin

test:
	bochs -f bochsrc

floppy:
	dd if=/dev/zero of=./floppy.img bs=512 count=2880
