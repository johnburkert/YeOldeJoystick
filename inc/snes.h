
#ifndef _SNES_H_
#define _SNES_H_

/**********************************************************
 * snes.h
 * John Burkert (johnburkert@gmail.com)
 *
 * The following websites were used as references for this project.
 * A very big THANK YOU to the authors for sharing their knowledge!!!
 *
 *   http://www.artlum.com/gameduino/nes.html
 *   http://pinouts.ru/Game/snescontroller_pinout.shtml
 *   http://www.slashdev.ca/2010/05/25/ps3-gamepad-with-home-button/
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
#define BUTTON_X        (1 << 2)
#define BUTTON_Y        (1 << 3)
#define BUTTON_SELECT   (1 << 4)
#define BUTTON_START    (1 << 5)
#define BUTTON_L1       (1 << 6)
#define BUTTON_R1       (1 << 7)

#define DPAD_UP         (1 << 0)
#define DPAD_DOWN       (1 << 1)
#define DPAD_LEFT       (1 << 2)
#define DPAD_RIGHT      (1 << 3)

#define VIRTUAL_PS      (1 << 0)
#define VIRTUAL_REBOOT  (1 << 1)
#define VIRTUAL_L2      (1 << 3)
#define VIRTUAL_R2      (1 << 4)

/**********************************************************
 * gamepad button defines
 */

#define GAMEPAD_CIRCLE_ON	    (BUTTON & BUTTON_A)
#define GAMEPAD_CROSS_ON	    (BUTTON & BUTTON_B)
#define GAMEPAD_TRIANGLE_ON	    (BUTTON & BUTTON_X)
#define GAMEPAD_SQUARE_ON	    (BUTTON & BUTTON_Y)

#define GAMEPAD_L1_ON	        (BUTTON & BUTTON_L1)
#define GAMEPAD_R1_ON	        (BUTTON & BUTTON_R1)
#define GAMEPAD_L2_ON	        (VIRTUAL & VIRTUAL_L2)
#define GAMEPAD_R2_ON	        (VIRTUAL & VIRTUAL_R2)

#define GAMEPAD_UP_ON	        (DPAD & DPAD_UP)
#define GAMEPAD_DOWN_ON	        (DPAD & DPAD_DOWN)
#define GAMEPAD_LEFT_ON         (DPAD & DPAD_LEFT)
#define GAMEPAD_RIGHT_ON	    (DPAD & DPAD_RIGHT)

#define GAMEPAD_START_ON		(BUTTON & BUTTON_START)
#define GAMEPAD_SELECT_ON	    (BUTTON & BUTTON_SELECT)

#define GAMEPAD_PS_ON		    (VIRTUAL & VIRTUAL_PS)
#define GAMEPAD_REBOOT_ON	    (VIRTUAL & VIRTUAL_REBOOT)

/**********************************************************
 * snes pinout
 *
 * -----
 * | 1 | VCC (white)    --->    VCC
 * | 2 | Clock (yellow) --->    PB0
 * | 3 | Latch (orange) --->    PB1
 * | 4 | Data (red)     --->    PB2
 * | 5 | empty
 * | 6 | empty
 * \ 7 / GND (brown)    --->    GND
 *  ---
 */

#define CLOCK   (1 << 0)
#define LATCH   (1 << 1)
#define DATA    (1 << 2)

/**********************************************************
 * initialize pins
 */ 

void gamepad_init(void) {
    DDRB |= CLOCK;  // output
    DDRB |= LATCH;  // output
    DDRB &= ~DATA;  // input

    PORTB |= CLOCK; // high output
    PORTB |= LATCH; // high output
    PORTB &= ~DATA; // normal input
}

/**********************************************************
 * read pins and set buttons masks
 */ 

void gamepad_read(void) {
    // set latch low
    PORTB &= ~LATCH;

    // read data pin, low is a press                cycle clock for next button
                                                    PORTB &= ~CLOCK; PORTB |= CLOCK;
    BUTTON  = (PINB & DATA) ? 0 : BUTTON_B;         PORTB &= ~CLOCK; PORTB |= CLOCK;
    BUTTON |= (PINB & DATA) ? 0 : BUTTON_Y;         PORTB &= ~CLOCK; PORTB |= CLOCK;
    BUTTON |= (PINB & DATA) ? 0 : BUTTON_SELECT;    PORTB &= ~CLOCK; PORTB |= CLOCK;
    BUTTON |= (PINB & DATA) ? 0 : BUTTON_START;     PORTB &= ~CLOCK; PORTB |= CLOCK;
    DPAD    = (PINB & DATA) ? 0 : DPAD_UP;          PORTB &= ~CLOCK; PORTB |= CLOCK;
    DPAD   |= (PINB & DATA) ? 0 : DPAD_DOWN;        PORTB &= ~CLOCK; PORTB |= CLOCK;
    DPAD   |= (PINB & DATA) ? 0 : DPAD_LEFT;        PORTB &= ~CLOCK; PORTB |= CLOCK;
    DPAD   |= (PINB & DATA) ? 0 : DPAD_RIGHT;       PORTB &= ~CLOCK; PORTB |= CLOCK;
    BUTTON |= (PINB & DATA) ? 0 : BUTTON_A;         PORTB &= ~CLOCK; PORTB |= CLOCK;
    BUTTON |= (PINB & DATA) ? 0 : BUTTON_X;         PORTB &= ~CLOCK; PORTB |= CLOCK;
    BUTTON |= (PINB & DATA) ? 0 : BUTTON_L1;        PORTB &= ~CLOCK; PORTB |= CLOCK;
    BUTTON |= (PINB & DATA) ? 0 : BUTTON_R1;        //PORTB &= ~CLOCK; PORTB |= CLOCK;

    // set latch high
    PORTB |= LATCH;

    // handle virtual buttons
    // ps       : select + start
    // reboot   : select + start + l1 + r1
    // l2       : select + l1
    // r2       : select + r1
    VIRTUAL = 0;
    if (GAMEPAD_SELECT_ON) {
        if (GAMEPAD_START_ON) {
            if (GAMEPAD_L1_ON && GAMEPAD_R1_ON) {
                // reboot
                VIRTUAL = VIRTUAL_REBOOT;
                // eat buttons
                BUTTON &= ~BUTTON_L1;
                BUTTON &= ~BUTTON_R1;
            } else {
                // ps
                VIRTUAL = VIRTUAL_PS;
            }

            // eat buttons
            BUTTON &= ~BUTTON_START;
            BUTTON &= ~BUTTON_SELECT;
        } else {
            if (GAMEPAD_L1_ON) {
                // l2
                VIRTUAL |= VIRTUAL_L2;
                // eat buttons
                BUTTON &= ~BUTTON_L1;
                BUTTON &= ~BUTTON_SELECT;
            }
            if (GAMEPAD_R1_ON) {
                // r2
                VIRTUAL |= VIRTUAL_R2;
                // eat buttons
                BUTTON &= ~BUTTON_R1;
                BUTTON &= ~BUTTON_SELECT;
            }
        }
    }
}

#endif

