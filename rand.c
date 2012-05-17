#include <avr/io.h>
#include <avr/interrupt.h>
#include "bits.h"
#include "rand.h"
#include "lcd.h"

static volatile int timer;
static unsigned long seed;

ISR(TIMER0_OVF_vect) {
    timer += 42;
}

void rand_init(void) {
    lcd_gotoxy(0, 0);
    lcd_puts_P("Init PRNG: ");
    SET_BIT(TIMSK, TOIE0);
    SET_BIT(TCCR0, CS00);
    sei();
    for (int i = 0; i < 10240; i += rand()%17) {
        lcd_gotoxy(11, 0);
        lcd_data(i*9&0xff);
        srand(timer);
    }
    lcd_clrscr();
    CLR_BIT(TIMSK, TOIE0);
    cli();
}

void srand(unsigned long s) {
     seed = s;
 } 

int rand(void) {
    seed = seed * 1664525L + 1013904223L;   
    return ((int)((seed>> 17) & 0x7fff));
}
