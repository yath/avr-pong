#include "debug.h"
#include "uart.h"
#include <stdio.h>
#ifndef NDEBUG
FILE debugf = FDEV_SETUP_STREAM(
    (int(*)(char, FILE*))uart_putc,
    NULL,
    _FDEV_SETUP_WRITE);

void debug_init(void) {
    stdout = stderr = &debugf;
}
#endif
