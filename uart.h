#ifndef _UART_H
#define _UART_H
#include <avr/pgmspace.h>
void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
void uart_puts_P(PGM_P s);
#endif
