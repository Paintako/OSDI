void uart_init();
void uart_send(unsigned int c); // sends a character over the serial line.
char uart_getc(); // receives a character. The carrige return character (13)
                  // will be converted into a newline character (10).
void uart_puts(char *s); // prints out a string. On newline, a carrige return
                         // character will also be sent (13 + 10).