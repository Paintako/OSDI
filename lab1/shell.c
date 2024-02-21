#include "my_string.h"
#include "uart.h"

void shell_init() {
  uart_puts("               _                            _                     "
            "                \r\n");
  uart_puts("              | |                          | |                    "
            "                \r\n");
  uart_puts(" __      _____| | ___ ___  _ __ ___   ___  | |__   ___  _ __ ___  "
            " ___            \r\n");
  uart_puts(" \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ | '_ \\ / _ \\| "
            "'_ ` _ \\ / _ \\\r\n");
  uart_puts("  \\ V  V /  __/ | (_| (_) | | | | | |  __/ | | | | (_) | | | | | "
            "|  __/          \r\n");
  uart_puts("   \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___| |_| |_|\\___/|_| "
            "|_| |_|\\___|   \r\n");
  uart_puts("Welcome to shell!\r\n");
}

void shell_ctrl(char *input) {
  // Remove newline
  input[strlen(input) - 1] = '\0';

  char arr[2][10] = {"hello", "help"};

  // compare with command list
  if (strcmp(input, arr[0]) == 0) {
    uart_puts("Hello, world!\r\n");
  } else if (strcmp(input, arr[1]) == 0) {
    uart_puts("Command list:\r\n");
    uart_puts("hello\r\n");
    uart_puts("help\r\n");
  } else {
    uart_puts("Command not found\r\n");
  }
}

void shell_read() {
  while (1) {
    uart_puts("$ ");
    char *input = uart_read_line();
    shell_ctrl(input);
  }
}
