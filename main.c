#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "common/common.h"
#include "rand_init.h"

#define BALL_LEFT BV(1)
#define BALL_UP   BV(0)           /*   .-  1 = left, 0 = right */
typedef enum {                    /*   v,- 1 = up, 0 down */
    DOWN_RIGHT = 0,               /* 0b00 */
    UP_RIGHT  = BALL_UP,          /* 0b01 */
    DOWN_LEFT = BALL_LEFT,        /* 0b10 */
    UP_LEFT   = BALL_UP|BALL_LEFT /* 0b11 */
} direction_t;

/* (0,0) is top left corner */
typedef uint8_t ball_x_t;
typedef uint8_t ball_y_t;
struct {
    ball_x_t x;
    ball_y_t y;
    direction_t dir;
} ball;

typedef uint16_t paddle_t;

#define PADDLE_TOP_BIT MSB(paddle_t)
#define PADDLE_BOT_BIT LSB(paddle_t)
#define PADDLE_TOP_BITV BV(PADDLE_TOP_BIT)
#define PADDLE_BOT_BITV BV(PADDLE_BOT_BIT)

#define BALL_MAX_X ((LCD_DISP_LENGTH*LCD_CHAR_WIDTH)-1)
#define BALL_MAX_Y PADDLE_TOP_BIT

#define LCD_CHAR_WIDTH 5
#define LCD_CHAR_HEIGHT 8

#if LCD_CHAR_HEIGHT != 8
#warn "LCD_CHAR_HEIGHT is not 8. things will break."
#endif

#define PADDLE_POS_P1 (1<<(LCD_CHAR_WIDTH-1)) /* leftmost */
#define PADDLE_POS_P2 (1)    /* rightmost */

#define PADDLE_LEN 5
#define PADDLE_LEN_BITS ((1<<PADDLE_LEN)-1)

#define BALL_CGCHAR 4
#define BALL_CGADDR (LCD_CHAR_HEIGHT*BALL_CGCHAR)

/* MSB is top of screen */
paddle_t paddle_p1, paddle_p2;


void cgaddr(uint8_t c) {
    lcd_command(BV(LCD_CGRAM)|c);
}

#undef DEBUG
#define DEBUG(...)

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
    int bline = -1;
    int bbit  = 0;
    if (ball.x < LCD_CHAR_WIDTH) { /* draw ball left */
        bline = PADDLE_TOP_BIT-ball.y;
        bbit  = BV(LCD_CHAR_WIDTH-ball.x);
        debug(C("bbit line "), d(bline), C(" left: "), d(bbit));
    }
    for (int i = PADDLE_TOP_BIT; i >= PADDLE_BOT_BIT; i--)
        lcd_data((paddle_p1 & BV(i) ? PADDLE_POS_P1 : 0) | (i == bline ? bbit : 0));
    if (ball.x > BALL_MAX_X-LCD_CHAR_WIDTH) { /* draw ball right */
        bline = PADDLE_TOP_BIT-ball.y;
        bbit  = BV(BALL_MAX_X-ball.x);
        debug(C("bbit line "), d(bline), C(" left: "), d(bbit));
    } else
        bline = -1;
    for (int i = PADDLE_TOP_BIT; i >= PADDLE_BOT_BIT; i--)
        lcd_data((paddle_p2 & BV(i) ? PADDLE_POS_P2 : 0) | (i == bline ? bbit : 0));
}

void paddle_up(paddle_t *paddle) {
    if (*paddle & PADDLE_TOP_BITV)
        return;
    *paddle <<= 1;
    draw_paddles();
}

void paddle_down(paddle_t *paddle) {
    if (*paddle & PADDLE_BOT_BITV)
        return;
    *paddle >>= 1;
    draw_paddles();
}

void new_game(void) {
    debug(C("Starting new game, random seed = "), x(rand()));
    /* Random paddle position */
    paddle_p1 = PADDLE_LEN_BITS <<
        (rand() % (PADDLE_TOP_BIT-PADDLE_LEN));
    paddle_p2 = paddle_p1;
    paddle_p2 = PADDLE_LEN_BITS <<
        (rand() % (PADDLE_TOP_BIT-PADDLE_LEN));

    /* Somewhere in the middle of the screen */
    ball.x = (rand() % (BALL_MAX_X-40))+20;
    ball.y = rand() % (BALL_MAX_Y-10)+5;
    ball.dir = rand()%4;

    debug(C("paddle_p1 = "), x(paddle_p1), C(", paddle_p2 = "), x(paddle_p2));
    debug(C("ball.x = "), d(ball.x), C(", .y = "), d(ball.y), C(", .dir = "), x(ball.dir));
    lcd_clrscr();
    /* clear ball */
    cgaddr(BALL_CGADDR);
    for (int i = 0; i < LCD_CHAR_HEIGHT; i++)
        lcd_data(0);
    draw_paddles();
}

void gameover(int winner) {
    lcd_clrscr();
    lcd_gotoxy(0, 0);
              /*0123456789ABCDEF */
    lcd_puts_P(PSTR("   GAME OVER!"));
    lcd_gotoxy(2, 1);
    lcd_puts_P(PSTR("Player "));
    lcd_data(winner+'0');
    lcd_puts_P(PSTR(" won"));
    _delay_ms(2000);
    new_game();
}

int move_ball(void) {
    int newx = ball.x;
    int newy = ball.y;
    int bounce = 0;
    if (ball.dir & BALL_UP) {
        if (--newy == 0) {
            bounce |= BALL_UP;
        }
    } else {
        if (++newy == BALL_MAX_Y) {
            bounce |= BALL_UP;
        }
    }

#   define ON_PADDLE(y, paddle) ((paddle) & BV(PADDLE_TOP_BIT-y))
    if (ball.dir & BALL_LEFT) {
        if (--newx == 0) {
            if (ON_PADDLE(ball.y, paddle_p1) || ON_PADDLE(newy, paddle_p1)) {
                newx++;
                bounce |= BALL_LEFT;
            } else {
                gameover(2);
                return 0;
            }
        }
    } else {
        if (++newx == BALL_MAX_X) {
            if (ON_PADDLE(ball.y, paddle_p2) || ON_PADDLE(newy, paddle_p2)) {
                newx--;
                bounce |= BALL_LEFT;
            } else {
                gameover(1);
                return 0;
            }
        }
    }
    debug(C("move ball from "), d(ball.x), c(','), d(ball.y), C(" to "),
        d(newx), c(','), d(newy), C(" (bounce "), d(bounce), C(")"));
    if (bounce)
        ball.dir ^= bounce;
    ball.x = newx;
    ball.y = newy;

    return 1; /* Move succeeded */
}

void move_and_draw_ball(void) {
#   define BALL_CHARPIX(prefix, thisball) \
        int prefix##x_c = (thisball).x / LCD_CHAR_WIDTH; \
        int prefix##x_p = LCD_CHAR_WIDTH - ((thisball).x % LCD_CHAR_WIDTH); \
        \
        int prefix##y_c = (thisball).y / LCD_CHAR_HEIGHT; \
        int prefix##y_p = (thisball).y % LCD_CHAR_HEIGHT;
#   define IS_NEAR_TO_PADDLE(x) \
        ((x) == 0 || (x) == LCD_DISP_LENGTH-1)

    BALL_CHARPIX(old_, ball);
    if (!move_ball())
        return;
    BALL_CHARPIX(new_, ball);

    if (IS_NEAR_TO_PADDLE(old_x_c) || IS_NEAR_TO_PADDLE(new_x_c)) {
        draw_paddles();
    }

    /* character boundary */
    if ((old_x_c != new_x_c) || (old_y_c != new_y_c)) {
        if (!IS_NEAR_TO_PADDLE(old_x_c)) {
            lcd_gotoxy(old_x_c, old_y_c);
            lcd_data(' ');
        }

        if (!IS_NEAR_TO_PADDLE(new_x_c)) {
            lcd_gotoxy(new_x_c, new_y_c);
            lcd_data(BALL_CGCHAR);
        }
    }

    (void)old_x_p; /* unused */

    if (!IS_NEAR_TO_PADDLE(old_x_c)) {
        cgaddr(BALL_CGADDR+old_y_p);
        lcd_data(0);
    }

    if (!IS_NEAR_TO_PADDLE(new_x_c)) {
        cgaddr(BALL_CGADDR+new_y_p);
        lcd_data(BV(new_x_p));
    }
}


int main(void) {
    uart_init();
    debug_init();
    lcd_init(LCD_DISP_ON);
    rand_init();

    new_game();

    int i = 0;

    while(1) {
        _delay_ms(50);
        move_and_draw_ball();
        if (++i == 4) {
            i = 0;
            switch (rand()%4) {
                case 0:
                    paddle_up(&paddle_p1);
                    break;
                case 1:
                    paddle_up(&paddle_p2);
                    break;
                case 2:
                    paddle_down(&paddle_p1);
                    break;
                case 3:
                    paddle_down(&paddle_p2);
            }
        }
    }
}
