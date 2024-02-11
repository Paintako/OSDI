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
8. Set AUX_MU_CNTL_REG to 3. Enable the transmitter and receiver.

*/

void uart_init(void) {
  *AUX_ENABLE |= 1;
  // Disable UART0
  *AUX_MU_CNTL = 0;
  // Disable interrupts
  *AUX_MU_IER = 0;
  // Enable 8 bit mode
  *AUX_MU_LCR = 3;
  // Set RTS line to be always high
  *AUX_MU_MCR = 0;
  // Set baud rate to 115200
  *AUX_MU_BAUD = 270;
  // Enable transmitter and receiver
  *AUX_MU_CNTL = 3;
  // Set AUX_MU_IIR_REG to 6. No FIFO.
  *AUX_MU_IIR = 6;
  // Set AUX_MU_CNTL_REG to 3. Enable the transmitter and receiver.
  *AUX_MU_CNTL = 3;
}

// get a character from the UART
char uart_getc(void) {
  // 1. Check AUX_MU_LSR_REG’s data ready field.
  while (1) {
    if (!(*AUX_MU_LSR & 0x01))
      break;
  }

  char r = (char)(*AUX_MU_IO);
  return r;
}

// send a character via the UART
void uart_send(unsigned int c) {
  // AUX_MU_LSR_REG register shows the data status.
  while (1) {
    // 0x20: Transmitter idle and FIFO is empty,
    if ((*AUX_MU_LSR & 0x20))
      break;
  }
  *AUX_MU_IO = c; // write the character to the buffer
}