
#ifndef _SATURN_H_
#define _SATURN_H_

/**********************************************************
 * saturn.h
 * John Burkert (johnburkert@gmail.com)
 *
 * The following websites were used as references for this project.
 * A very big THANK YOU to the authors for sharing their knowledge!!!
 *
 *   http://www.raphnet.net/electronique/saturn_usb/index_en.php
 *   http://www.gamesx.com/controldata/saturn.htm
 *   http://www.slashdev.ca/2010/05/22/simple-6-button-ps3-gamepad/
 *   http://www.pjrc.com/teensy/index.html
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**********************************************************
 * snes button masks and defines
 */

uint8_t BUTTON = 0;
uint8_t DPAD = 0;
uint8_t VIRTUAL = 0;

#define BUTTON_A        (1 << 0)
#define BUTTON_B        (1 << 1)
#define BUTTON_C        (1 << 2)
#define BUTTON_X        (1 << 3)
#define BUTTON_Y        (1 << 4)
#define BUTTON_Z        (1 << 5)
#define BUTTON_L        (1 << 6)
#define BUTTON_R        (1 << 7)

#define DPAD_UP         (1 << 0)
#define DPAD_DOWN       (1 << 1)
#define DPAD_LEFT       (1 << 2)
#define DPAD_RIGHT      (1 << 3)

#define VIRTUAL_PS      (1 << 0)
#define VIRTUAL_REBOOT  (1 << 1)
#define VIRTUAL_SELECT  (1 << 2)
#define VIRTUAL_START   (1 << 3)

/**********************************************************
 * gamepad button defines
 */

#define GAMEPAD_CIRCLE_ON	    (BUTTON & BUTTON_B)
#define GAMEPAD_CROSS_ON	    (BUTTON & BUTTON_A)
#define GAMEPAD_TRIANGLE_ON	    (BUTTON & BUTTON_Y)
#define GAMEPAD_SQUARE_ON	    (BUTTON & BUTTON_X)

#define GAMEPAD_L1_ON	        (BUTTON & BUTTON_L)
#define GAMEPAD_L2_ON	        (BUTTON & BUTTON_Z)
#define GAMEPAD_R1_ON	        (BUTTON & BUTTON_R)
#define GAMEPAD_R2_ON	        (BUTTON & BUTTON_C)

#define GAMEPAD_UP_ON	        (DPAD & DPAD_UP)
#define GAMEPAD_DOWN_ON	        (DPAD & DPAD_DOWN)
#define GAMEPAD_LEFT_ON         (DPAD & DPAD_LEFT)
#define GAMEPAD_RIGHT_ON	    (DPAD & DPAD_RIGHT)

#define GAMEPAD_START_ON		(VIRTUAL & VIRTUAL_START)
#define GAMEPAD_SELECT_ON	    (VIRTUAL & VIRTUAL_SELECT)

#define GAMEPAD_PS_ON		    (VIRTUAL & VIRTUAL_PS)
#define GAMEPAD_REBOOT_ON	    (VIRTUAL & VIRTUAL_REBOOT)

/**********************************************************
 * saturn pinout
 *
 *  ----
 * / 1 | VCC (blue)     --->    VCC
 * | 2 | D1 (green)     --->    PB1
 * | 3 | D0 (black)     --->    PB0
 * | 4 | S0 (orange)    --->    PD0
 * | 5 | S1 (red)       --->    PD1 
 * | 6 | 5V (brown)     --->    (unused)
 * | 7 | D3 (yellow)    --->    PB3
 * | 8 | D2 (gray)      --->    PB2
 * \ 9 | GND (white)    --->    GND
 *  ----
 */

#define DATA0   (1 << 0)
#define DATA1   (1 << 1)
#define DATA2   (1 << 2)
#define DATA3   (1 << 3)
#define SELECT0 (1 << 0)
#define SELECT1 (1 << 1)

/**********************************************************
 * initialize pins
 */ 

void gamepad_init(void) {
    DDRB &= ~DATA0;     // input
    DDRB &= ~DATA1;     // input
    DDRB &= ~DATA2;     // input
    DDRB &= ~DATA3;     // input
    DDRD |= SELECT0;    // output
    DDRD |= SELECT1;    // output

    PORTB |= DATA0;     // input pullup
    PORTB |= DATA1;     // input pullup
    PORTB |= DATA2;     // input pullup
    PORTB |= DATA3;     // input pullup
    PORTD |= SELECT0;   // high output
    PORTD |= SELECT1;   // high output
}

/**********************************************************
 * read pins and set buttons masks
 */ 

void gamepad_read(void) {
    // physical start is 'special'
    uint8_t special = 0;
    // set output high, high
    PORTD |= SELECT0;
    PORTD |= SELECT1;
    _delay_ms(1);
    // read pins
    BUTTON = (PINB & DATA3) ? 0 : BUTTON_L;
    // set output low, low
    PORTD &= ~SELECT0;
    PORTD &= ~SELECT1;
    _delay_ms(1);
    // read pins
    BUTTON |= (PINB & DATA0) ? 0 : BUTTON_Z;
    BUTTON |= (PINB & DATA1) ? 0 : BUTTON_Y;
    BUTTON |= (PINB & DATA2) ? 0 : BUTTON_X;
    BUTTON |= (PINB & DATA3) ? 0 : BUTTON_R;
    // set output high, low
    PORTD |= SELECT0;
    PORTD &= ~SELECT1;
    _delay_ms(1);
    // read pins
    BUTTON |= (PINB & DATA0) ? 0 : BUTTON_B;
    BUTTON |= (PINB & DATA1) ? 0 : BUTTON_C;
    BUTTON |= (PINB & DATA2) ? 0 : BUTTON_A;
    special = (PINB & DATA3) ? 0 : 1;
    // set output low, high
    PORTD &= ~SELECT0;
    PORTD |= SELECT1;
    _delay_ms(1);
    // read pins
    DPAD  = (PINB & DATA0) ? 0 : DPAD_UP;
    DPAD |= (PINB & DATA1) ? 0 : DPAD_DOWN;
    DPAD |= (PINB & DATA2) ? 0 : DPAD_LEFT;
    DPAD |= (PINB & DATA3) ? 0 : DPAD_RIGHT;

    // handle virtual buttons
    // select   : special + l1
    // start    : special + r1
    // ps       : select + start
    // reboot   : select + start + l2 + r2
    VIRTUAL = 0;
    if (special) {
        if (GAMEPAD_L1_ON && GAMEPAD_R1_ON) {
            if (GAMEPAD_L2_ON && GAMEPAD_R2_ON) {
                // reboot
                VIRTUAL = VIRTUAL_REBOOT;
                // eat buttons
                BUTTON &= ~BUTTON_Z;
                BUTTON &= ~BUTTON_C;
            } else {
                // ps
                VIRTUAL = VIRTUAL_PS;
            }
            // eat buttons
            BUTTON &= ~BUTTON_L;
            BUTTON &= ~BUTTON_R;
        } else if (GAMEPAD_L1_ON) {
            // select
            VIRTUAL |= VIRTUAL_SELECT;
            // eat buttons
            BUTTON &= ~BUTTON_L;
        } else if (GAMEPAD_R1_ON) {
            // start
            VIRTUAL |= VIRTUAL_START;
            // eat buttons
            BUTTON &= ~BUTTON_R;
        }
    }
}

#endif

