#include "uart.h"

void main() {
  uart_init();
  // say hello
  uart_send('H');

  // echo everything back
  while (1) {
    uart_send(uart_getc());
  }
}