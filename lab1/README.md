# LAB1

## Basic Exercise 1

When a program is loaded, it requires,

- All it’s data is presented at correct memory address.
- The program counter is set to correct memory address.
- The bss segment are initialized to 0.
- The stack pointer is set to a proper address.

After rpi3 booting, its booloader loads kernel8.img to physical address 0x80000, and start executing the loaded program. If the linker script is correct, the above two requirements are met.

However, both bss segment and stack pointer are not properly initialized. Hence, you need to initialize them at very beginning. Otherwise, it may lead to undefined behaviors.

## Mini Uart



## Shell