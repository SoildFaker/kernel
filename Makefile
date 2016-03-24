#!Kernel Makefile
#

C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.s")
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))

CC = gcc
LD = ld
ASM = as

C_FLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin -fno-stack-protector -I include
LD_FLAGS = -T scripts/kernel.ld -m elf_i386 -nostdlib
ASM_FLAGS = -o

all: $(S_OBJECTS) $(C_OBJECTS) link update_image

# The automatic variable `$<' is just the first prerequisite
.c.o:
	@echo compiling c $< ...
	$(CC) $(C_FLAGS) $< -o $@

.s.o:
	@echo compiling asm $< ...
	$(ASM) $(ASM_FLAGS) $<

link:
	@echo linking...
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o kernel

.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) kernel

.PHONY:update_image
update_image:
	sudo mount hdc.img /mnt/kernel
	sudo cp kernel /mnt/kernel/kernel
	sleep 1
	sudo umount /mnt/kernel

.PHONY:debug
debug:
	qemu-system-i386 -S -s -fda hdc.img -boot a &
	sleep 1
	cgdb -x scripts/gdbinit

.PHONY:qemu
qemu:
	qemu-system-i386 -fda hdc.img -boot a

.PHONY:bochs
bochs:
	bochs -f scripts/bochsrc.txt

.PHONY:mount_image
mount_image:
	sudo mount hdc.img /mnt/kernel

.PHONY:umount_image
umount_image:
	sudo umount /mnt/kernel

