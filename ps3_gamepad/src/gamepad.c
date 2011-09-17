/* PS3 Teensy HID Gamepad
 * Copyright (C) 2010 Josh Kropf <josh@slashdev.ca>
 *
 * Based on works by:
 *   grunskis <http://github.com/grunskis/gamepad>
 *   Toodles <http://forums.shoryuken.com/showthread.php?t=131230>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Keyboard example for Teensy USB Development Board
 * http://www.pjrc.com/teensy/usb_keyboard.html
 * Copyright (c) 2008 PJRC.COM, LLC
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

#include <avr/io.h>
#include <util/delay.h>
#include "usb_gamepad.h"
#include "reboot.h"

#if defined(SNES)
#include "snes.h"
#elif defined(SATURN)
#include "saturn.h"
#else
#error "Undefined gamepad"
#endif

#define LED_CONFIG	(DDRD |= (1<<6))
#define LED_OFF	(PORTD &= ~(1<<6))
#define LED_ON	(PORTD |= (1<<6))

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

int main(void) {
	// set for 16 MHz clock
	CPU_PRESCALE(0);

    // initialize gamepad
    gamepad_init();

	LED_CONFIG;
	LED_ON; // power up led on startup for 1 sec

	// Initialize the USB, and then wait for the host to set configuration.
	// If the Teensy is powered without a PC connected to the USB port,
	// this will wait forever.
	usb_init();
	while (!usb_configured()) /* wait */ ;

	// Wait an extra second for the PC's operating system to load drivers
	// and do whatever it does to actually be ready for input
	_delay_ms(1000);

	LED_OFF;

	while (1) {
		usb_gamepad_reset_state();

        // read gamepad data
        gamepad_read();

        if (BTN_REBOOT_ON)
            reboot();

		if (JOYSTICK_UP_ON) {
			gamepad_state.direction = 0;
			if (JOYSTICK_LEFT_ON) {
				gamepad_state.direction = 7;
			} else if (JOYSTICK_RIGHT_ON) {
				gamepad_state.direction = 1;
			}
		} else {
			if (JOYSTICK_DOWN_ON) {
				gamepad_state.direction = 4;
				if (JOYSTICK_LEFT_ON) {
					gamepad_state.direction = 5;
				} else if (JOYSTICK_RIGHT_ON) {
					gamepad_state.direction = 3;
				}
			} else {
				if (JOYSTICK_LEFT_ON) {
					gamepad_state.direction = 6;
				} else if (JOYSTICK_RIGHT_ON) {
					gamepad_state.direction = 2;
				}
			}
		}

		if (BTN_CROSS_ON) {
			gamepad_state.cross_btn = 1;
			gamepad_state.cross_axis = 0xff;
		}

		if (BTN_SQUARE_ON) {
			gamepad_state.square_btn = 1;
			gamepad_state.square_axis = 0xff;
		}

		if (BTN_TRIANGLE_ON) {
			gamepad_state.triangle_btn = 1;
			gamepad_state.triangle_axis = 0xff;
		}

		if (BTN_CIRCLE_ON) {
			gamepad_state.circle_btn = 1;
			gamepad_state.circle_axis = 0xff;
		}

		if (BTN_R1_ON) {
			gamepad_state.r1_btn = 1;
			gamepad_state.r1_axis = 0xff;
		}

		if (BTN_R2_ON) {
			gamepad_state.r2_btn = 1;
			gamepad_state.r2_axis = 0xff;
		}

		if (BTN_L1_ON) {
			gamepad_state.l1_btn = 1;
			gamepad_state.l1_axis = 0xff;
		}

		if (BTN_L2_ON) {
			gamepad_state.l2_btn = 1;
			gamepad_state.l2_axis = 0xff;
		}

		if (BTN_START_ON) {
			gamepad_state.start_btn = 1;
		}

		if (BTN_SELECT_ON) {
			gamepad_state.select_btn = 1;
		}

		if (BTN_PS_ON) {
			gamepad_state.ps_btn = 1;
		}

		usb_gamepad_send();
	}
}

