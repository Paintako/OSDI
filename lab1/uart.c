#include "gpio.h"

/*
GPIO 14, 15 can be both used for mini UART and PL011 UART. However, mini UART
should set ALT5 and PL011 UART should set ALT0. You need to configure GPFSELn
register to change alternate function.
*/

#define AUX_ENABLE ((volatile unsigned int *)(MMIO_BASE + 0x00215004))
#define AUX_MU_IO ((volatile unsigned int *)(MMIO_BASE + 0x00215040))
#define AUX_MU_IER ((volatile unsigned int *)(MMIO_BASE + 0x00215044))
#define AUX_MU_IIR ((volatile unsigned int *)(MMIO_BASE + 0x00215048))
#define AUX_MU_LCR ((volatile unsigned int *)(MMIO_BASE + 0x0021504C))
#define AUX_MU_MCR ((volatile unsigned int *)(MMIO_BASE + 0x00215050))
#define AUX_MU_LSR ((volatile unsigned int *)(MMIO_BASE + 0x00215054))
#define AUX_MU_MSR ((volatile unsigned int *)(MMIO_BASE + 0x00215058))
#define AUX_MU_SCRATCH ((volatile unsigned int *)(MMIO_BASE + 0x0021505C))
#define AUX_MU_CNTL ((volatile unsigned int *)(MMIO_BASE + 0x00215060))
#define AUX_MU_STAT ((volatile unsigned int *)(MMIO_BASE + 0x00215064))
#define AUX_MU_BAUD ((volatile unsigned int *)(MMIO_BASE + 0x00215068))

/*
Initialization:
1. Set AUXENB register to enable mini UART. Then mini UART register can be
accessed.
2. Set AUX_MU_CNTL_REG to 0. Disable transmitter and receiver during
configuration.
3. Set AUX_MU_IER_REG to 0. Disable interrupt because currently you don’t need
interrupt.
4. Set AUX_MU_LCR_REG to 3. Set the data size to 8 bit.
5. Set AUX_MU_MCR_REG to 0. Don’t need auto flow control.
6. Set AUX_MU_BAUD to 270. Set baud rate to 115200
    After booting, the system clock is 250 MHz.
7. Set AUX_MU_IIR_REG to 6. No FIFO.
8. Map UART to GPIO pins.
9. Set AUX_MU_CNTL_REG to 3. Enable the transmitter and receiver.

*/

void uart_init() {
  register unsigned int r;

  /* initialize UART */
  *AUX_ENABLE |= 1;   // enable UART1, AUX mini uart
  *AUX_MU_CNTL = 0;   // Disable TX, RX during configuration (TX means transmit,
                      // RX means receive)
  *AUX_MU_IER = 0;    // Disable interrupt
  *AUX_MU_LCR = 3;    // set data size to 8 bits
  *AUX_MU_MCR = 0;    // don't need auto flow control
  *AUX_MU_BAUD = 270; // 115200 baud
  *AUX_MU_IIR = 6;    // disable interrupts

  /* map UART1 to GPIO pins */
  // 1. Set GPIO 14 and 15 to alternative function 5, which is UART
  r = *GPFSEL1;
  r &= ~((7 << 12) | (7 << 15)); // Reset GPIO 14 and 15
  r |= (2 << 12) | (2 << 15);    // Set alt5
  *GPFSEL1 = r;

  // 2. Disable GPIO pull up/down (Because these GPIO pins use alternate
  // functions, not basic input-output) Set control signal to disable
  *GPPUD = 0; // enable pins 14 and 15

  // Wait 150 cycles
  r = 150;
  while (r--) {
    asm volatile("nop");
  }

  // Clock the control signal into the GPIO pads
  *GPPUDCLK0 = (1 << 14) | (1 << 15);
  r = 150;
  while (r--) {
    asm volatile("nop");
  }

  // Remove the clock and control signal
  *GPPUDCLK0 = 0;   // flush GPIO setup
  *AUX_MU_CNTL = 3; // enable Tx, Rx
}

// read a character from the UART
char uart_read() {
  // Check data ready field
  do {
    asm volatile("nop");
  } while (!(*AUX_MU_LSR & 0x01));
  // Read
  char r = (char)(*AUX_MU_IO);
  // Convert carrige return to newline
  return r;
}

// read whole line from the UART using uart_read
char *uart_read_line() {
  static char buffer[128]; // Assuming maximum input length is 128 characters
  char *ptr = buffer;

  // Read characters until newline is encountered or buffer is full
  while (1) {
    // Check data ready field
    char r = uart_read();

    // Convert carrige return to newline
    if (r == '\r') {
      *ptr++ = '\n'; // Change carriage return to newline
      *ptr = '\0';   // Null-terminate the string
      uart_write('\n');
      break; // Exit the loop
    } else {
      *ptr++ = r; // Store the character in buffer
      uart_write(r);
    }
  }

  return buffer;
}

// write a character to the UART
void uart_write(unsigned int c) {

  // Check transmitter idle field
  do {
    asm volatile("nop");
  } while (!(*AUX_MU_LSR & 0x20));
  // Write
  *AUX_MU_IO = c;
}

// Print a string to the UART
void uart_printString(char *s) {
  // use a while loop to print each character of the string to the UART
  while (*s) {
    // convert newline to carrige return + newline
    if (*s == '\n') {
      uart_write('\r');
    }
    uart_write(*s++);
  }
}

// Print a hex number to the UART
void uart_hex(unsigned int d) {
  unsigned int n;
  int c;
  for (c = 28; c >= 0; c -= 4) {
    // get highest tetrad
    n = (d >> c) & 0xF;
    // 0-9 => '0'-'9', 10-15 => 'A'-'F'
    n += n > 9 ? 0x37 : 0x30;
    uart_printString(n);
  }
}