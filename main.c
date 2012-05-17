#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "bits.h"
#include "lcd.h"
#include "rand.h"

typedef enum {
    DOWN_RIGHT,
    UP_RIGHT,
    DOWN_LEFT,
    UP_LEFT
} direction_t;

struct {
    uint8_t x;
    uint8_t y;
    direction_t dir;
} ball;

typedef uint16_t paddle_t;

#define PADDLE_POS_P1 (1<<4) /* leftmost */
#define PADDLE_POS_P2 (1)    /* rightmost */

/* MSB is top of screen */
paddle_t paddle_p1 = 0x0f00,
         paddle_p2 = 0x00f0;

void cgaddr(uint8_t c) {
    lcd_command(BV(LCD_CGRAM)|c);
}

void paddle_up(paddle_t *paddle) {
    if (*paddle & BV(15))
        return;
    *paddle <<= 1;
    draw_paddles();
}

void paddle_down(paddle_t *paddle) {
    if (*paddle & BV(0))
        return;
    *paddle >>= 1;
    draw_paddles();
}

void draw_paddles(void) {
    /* user-defined characters 00-01 are paddle p1,
     * 02-03 paddle p2 */
    lcd_gotoxy(0, 0);
    lcd_data(0);
    lcd_gotoxy(0, 1);
    lcd_data(1);
    lcd_gotoxy(LCD_DISP_LENGTH-1, 0);
    lcd_data(2);
    lcd_gotoxy(LCD_DISP_LENGTH-1, 1);
    lcd_data(3);
    cgaddr(0);
    for (int i = 15; i >= 0; i--)
        lcd_data(paddle_p1 & BV(i) ? PADDLE_POS_P1 : 0);
    for (int i = 15; i >= 0; i--)
        lcd_data(paddle_p2 & BV(i) ? PADDLE_POS_P2 : 0);
}


int main(void) {
    lcd_init(LCD_DISP_ON);
    rand_init();
    draw_paddles();

    while(1) {
        _delay_ms(100);
        switch (rand()%4) {
            case 0:
                paddle_up(&paddle_p1);
                break;
            case 1:
                paddle_down(&paddle_p1);
                break;
            case 2:
                paddle_up(&paddle_p2);
                break;
            case 3:
                paddle_down(&paddle_p2);
        }
    }
}
