
#ifndef _SNES_H_
#define _SNES_H_

/* snes.h
 * John Burkert (johnburkert@gmail.com)
 *
 * The following websites were used as references for this project.
 * A very big THANK YOU to the authors for sharing their knowledge!!!
 *
 *   http://www.artlum.com/gameduino/nes.html
 *   http://pinouts.ru/Game/snescontroller_pinout.shtml
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

///////////////////////////////////////////////////////////
// snes button masks and defines

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

///////////////////////////////////////////////////////////
// gamepad.c defines

#define BTN_CIRCLE_ON	    (BUTTON & BUTTON_A)
#define BTN_CROSS_ON	    (BUTTON & BUTTON_B)
#define BTN_TRIANGLE_ON	    (BUTTON & BUTTON_X)
#define BTN_SQUARE_ON	    (BUTTON & BUTTON_Y)

#define BTN_L1_ON	        (BUTTON & BUTTON_L1)
#define BTN_R1_ON	        (BUTTON & BUTTON_R1)
#define BTN_L2_ON	        (VIRTUAL & VIRTUAL_L2)
#define BTN_R2_ON	        (VIRTUAL & VIRTUAL_R2)

#define JOYSTICK_UP_ON	    (DPAD & DPAD_UP)
#define JOYSTICK_DOWN_ON	(DPAD & DPAD_DOWN)
#define JOYSTICK_LEFT_ON	(DPAD & DPAD_LEFT)
#define JOYSTICK_RIGHT_ON	(DPAD & DPAD_RIGHT)

#define BTN_START_ON		(BUTTON & BUTTON_START)
#define BTN_SELECT_ON	    (BUTTON & BUTTON_SELECT)

#define BTN_PS_ON		    (VIRTUAL & VIRTUAL_PS)
#define BTN_REBOOT_ON	    (VIRTUAL & VIRTUAL_REBOOT)

///////////////////////////////////////////////////////////
// snes pins

#define CLOCK   (1 << 0)
#define LATCH   (1 << 1)
#define DATA    (1 << 2)

void gamepad_init(void) {
    DDRB |= CLOCK;  // output
    DDRB |= LATCH;  // output
    DDRB &= ~DATA;  // input

    PORTB |= CLOCK; // high output
    PORTB |= LATCH; // high output
    PORTB &= ~DATA; // normal input
}

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
    if (BTN_SELECT_ON) {
        if (BTN_START_ON) {
            if (BTN_L1_ON && BTN_R1_ON) {
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
            if (BTN_L1_ON) {
                // l2
                VIRTUAL |= VIRTUAL_L2;
                // eat buttons
                BUTTON &= ~BUTTON_L1;
                BUTTON &= ~BUTTON_SELECT;
            }
            if (BTN_R1_ON) {
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

