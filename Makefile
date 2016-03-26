#!Kernel Makefile
#

C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.s")
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))

CC = gcc
LD86 = ld86
LD = ld
AS86 = as86
ASM = as

C_FLAGS = -c -Wall -ggdb -gstabs+ -nostdinc -fno-builtin -fno-stack-protector -I include
LD86_FLAGS = -0 -s -o
LD_FLAGS = 
AS86_FLAGS = -0 -a -o
ASM_FLAGS =

all: create_bootloader update_image

# The automatic variable `$<' is just the first prerequisite
.c.o:
	@echo compiling c $< ...
	$(CC) $(C_FLAGS) $< -o $@

.s.o:
	@echo compiling as86 $< ...
	$(ASM) $(ASM_FLAGS) $<

link:
	@echo create bootloader with ld86...
	$(LD86) $(LD86_FLAGS) boot $(BOOT_OBJ)

create_bootloader:
	@echo compiling with as86 ...
	$(AS86) $(AS86_FLAGS) ./boot/boot.o ./boot/boot.s
	@echo create bootloader with ld86...
	$(LD86) $(LD86_FLAGS) ./boot/boot ./boot/boot.o

.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) _kernel

.PHONY:update_image
update_image:
	dd bs=32 if=./boot/boot of=./hdc.img skip=1
	sleep 1

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

