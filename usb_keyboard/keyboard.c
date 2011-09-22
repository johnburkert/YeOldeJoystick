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
#include "usb_keyboard.h"
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

/**********************************************************
 * keyboard
 */

uint8_t keymask;

void key_press(uint8_t key, uint8_t press) {
    // check if key is already in array
    int i;
    for (i = 0; i < 6; i++) {
        if (key == keyboard_keys[i])
            break;
    }
    // if key is in array and it is released, update array value
    if (i < 6 && !press) {
        // remove value
        keyboard_keys[i] = 0;
    // if key is pressed and there is space in the array, add value to array
    } else if (press && keymask) {
        // find avail index
        for (i = 0; i < 6; i++) {
            if (keymask & (1 << i)) {
                // set value
                keyboard_keys[i] = key;
                // remove from keymask
                keymask &= ~(1 << i);
                break;
            }
        }
    }
}

void usb_keyboard_reset_state(void) {
    // set keymask bit high if keyboard_keys[] is unused
    keymask = 0;
    for (int i = 0; i < 6; i++) {
        if (keyboard_keys[i] == 0) {
            keymask |= (1 << i);
        }
    }
}

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
		usb_keyboard_reset_state();

        // read gamepad data
        gamepad_read();

        if (GAMEPAD_REBOOT_ON)
            reboot();

        key_press(KEY_UP,       GAMEPAD_UP_ON);
        key_press(KEY_DOWN,     GAMEPAD_DOWN_ON);
        key_press(KEY_LEFT,     GAMEPAD_LEFT_ON);
        key_press(KEY_RIGHT,    GAMEPAD_RIGHT_ON);
        key_press(KEY_1,        GAMEPAD_CIRCLE_ON);
        key_press(KEY_2,        GAMEPAD_CROSS_ON);
        key_press(KEY_3,        GAMEPAD_TRIANGLE_ON);
        key_press(KEY_4,        GAMEPAD_SQUARE_ON);
        key_press(KEY_5,        GAMEPAD_L1_ON);
        key_press(KEY_6,        GAMEPAD_R1_ON);
        key_press(KEY_7,        GAMEPAD_L2_ON);
        key_press(KEY_8,        GAMEPAD_R2_ON);
        key_press(KEY_9,        GAMEPAD_START_ON);
        key_press(KEY_0,        GAMEPAD_SELECT_ON);
        key_press(KEY_HOME,     GAMEPAD_PS_ON);

		usb_keyboard_send();
	}
}

