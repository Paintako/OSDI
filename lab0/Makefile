# Created by Paintakotako on 2024/02/05

AARCH_PREFIX = aarch64-linux-gnu
CC = $(AARCH_PREFIX)-gcc
LD = $(AARCH_PREFIX)-ld
OBJCOPY = $(AARCH_PREFIX)-objcopy

# -nostdlib: Do not use the standard system startup files or libraries when linking.
# -nostartfiles: Do not use the standard system startup files when linking.
# -ffreestanding: Assert that the standard library may not exist, and that main is the entry point. ( from aarch64-linux-gnu manual )
CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles

# wildcard: find all files with specific suffix, *.s means all files with suffix .s
ASSEMBLY_FILES = $(wildcard *.S)

# make all to generate kernel8.img
all: kernel8.img

# kernel8.img is the final output file
kernel8.img: $(ASSEMBLY_FILES)
	$(CC) $(CFLAGS) -c $(ASSEMBLY_FILES)
	$(LD) -T linker.ld -o kernel8.img *.o

.PHONY: clean
clean:
	rm -f *.o *.elf *.bin

run: 
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -display none -d in_asm  

