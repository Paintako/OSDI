# Created by Paintakotako on 2024/02/05

AARCH_PREFIX = aarch64-linux-gnu
CC = $(AARCH_PREFIX)-gcc
LD = $(AARCH_PREFIX)-ld -Map=kernel8.map
OBJCOPY = $(AARCH_PREFIX)-objcopy

# -nostdlib: Do not use the standard system startup files or libraries when linking.
# -nostartfiles: Do not use the standard system startup files when linking.
# -ffreestanding: Assert that the standard library may not exist, and that main is the entry point. ( from aarch64-linux-gnu manual )
CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles

# wildcard: find all files with specific suffix, *.s means all files with suffix .s
CFILES = $(wildcard *.c)
ASSEMBLY_FILES = $(wildcard *.S)
LINKER_FILES = $(wildcard *.ld)
OBJS = $(CFILES:.c=.o)

# make all to build kernel8.img
all: kernel8.img


# Note: $^ means dependency files, $@ means target file
# For example: kernel8.img: $(ASSEMBLY_FILES) 
# means kernel8.img depends on all files with suffix .S
# $^ means all files with suffix (dependencies) .S, $@ means kernel8.img (target file)
kernel8.img: $(ASSEMBLY_FILES) $(OBJS)
	$(CC) $(CFLAGS) -c $^
	$(LD) -T $(LINKER_FILES) -o $@ *.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *.elf *.bin *.img *.map

run: 
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial null -serial stdio

