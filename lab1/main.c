#include "shell.h"
#include "uart.h"
void main() {
  // set up serial console
  uart_init();

  // shell
  shell_init();
  shell_read();

  return;
}