#include <avr/io.h>
#include <avr/pgmspace.h>
#include "uart.h"
#include "bits.h"

void uart_init(void) {
    /* Set baud rate (9600) */
#   define BAUD 9600
#   include <util/setbaud.h>
    UBRRH = ~BV(URSEL) & /* select UBRRH register */
            UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;
#   if USE_2X /* output from setbaud.h */
    SET_BIT(UCSRA, U2X);
#   else
    CLR_BIT(UCSRA, U2X);
#   endif
#   undef BAUD

    SET_BITS(UCSRB, TXEN, RXEN); /* enable rx and tx */
    CLR_BIT(UCSRB, UCSZ2); /* 8 bit */

    SET_BIT(UCSRC, URSEL, /* select UCSRC register */
            UCSZ1, UCSZ0);  /* 8 bit */
    CLR_BITS(UCSRC, UPM0, USBS, UMSEL); /* no parity, 1 stop-bit, async */
}

void uart_putc(char c) {
    /* replace \n by \r\n */
    if (c == '\n')
        uart_putc('\r');
    /* wait while data register not empty */
    while (!TST_BIT(UCSRA, UDRE)) ;
    UDR = c;
}

void uart_puts(const char *s) {
    for (const char *p = s; *p != '\0'; p++)
        uart_putc(*p);
}

void uart_puts_P(PGM_P s) {
    PGM_P p = s;
    char c;
    while ((c = pgm_read_byte(p++)) != '\0')
        uart_putc(c);
}
