/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"

#ifdef SDL_JOYSTICK_TINSPIRE

/* This is the system specific header for the SDL joystick API */

#include "SDL_events.h"
#include "SDL_joystick.h"
#include "../SDL_sysjoystick.h"
#include "../SDL_joystick_c.h"

static t_key js_keymap[NSP_NUMBUTTONS];

static char ja_state[NSP_NUMAXES] = {SDL_RELEASED};
static char jb_state[NSP_NUMBUTTONS] = {SDL_RELEASED};

void nsp_init_js_keymap(void) {
	js_keymap[NSP_JB_ESC] =		KEY_NSPIRE_ESC;
	js_keymap[NSP_JB_SCRATCHPAD] =	KEY_NSPIRE_SCRATCHPAD;
	js_keymap[NSP_JB_TAB] =		KEY_NSPIRE_TAB;
	js_keymap[NSP_JB_HOME] =	KEY_NSPIRE_HOME;
	js_keymap[NSP_JB_DOC] =		KEY_NSPIRE_DOC;
	js_keymap[NSP_JB_MENU] =	KEY_NSPIRE_MENU;
	js_keymap[NSP_JB_CTRL] =	KEY_NSPIRE_CTRL;
	js_keymap[NSP_JB_SHIFT] =	KEY_NSPIRE_SHIFT;
	js_keymap[NSP_JB_VAR] =		KEY_NSPIRE_VAR;
	js_keymap[NSP_JB_DEL] =		KEY_NSPIRE_DEL;
	js_keymap[NSP_JB_0] =		KEY_NSPIRE_0;
	js_keymap[NSP_JB_1] =		KEY_NSPIRE_1;
	js_keymap[NSP_JB_2] =		KEY_NSPIRE_2;
	js_keymap[NSP_JB_3] =		KEY_NSPIRE_3;
	js_keymap[NSP_JB_4] =		KEY_NSPIRE_4;
	js_keymap[NSP_JB_5] =		KEY_NSPIRE_5;
	js_keymap[NSP_JB_6] =		KEY_NSPIRE_6;
	js_keymap[NSP_JB_7] =		KEY_NSPIRE_7;
	js_keymap[NSP_JB_8] =		KEY_NSPIRE_8;
	js_keymap[NSP_JB_9] =		KEY_NSPIRE_9;
	js_keymap[NSP_JB_PERIOD] =	KEY_NSPIRE_PERIOD;
	js_keymap[NSP_JB_NEGATIVE] =	KEY_NSPIRE_NEGATIVE;
	js_keymap[NSP_JB_ENTER] =	KEY_NSPIRE_ENTER;
	js_keymap[NSP_JB_CLICK] =	KEY_NSPIRE_CLICK;
}

/* Function to scan the system for joysticks.
 * This function should set SDL_numjoysticks to the number of available
 * joysticks.  Joystick 0 should be the system default joystick.
 * It should return 0, or -1 on an unrecoverable fatal error.
 */
int SDL_SYS_JoystickInit(void)
{
	nsp_init_js_keymap();
	SDL_numjoysticks = 1;
	return(1);
}

/* Function to get the device-dependent name of a joystick */
const char *SDL_SYS_JoystickName(int index)
{
	return index == 0 ? "TI-Nspire built-in joystick" : NULL;
}

/* Function to open a joystick for use.
   The joystick to open is specified by the index field of the joystick.
   This should fill the nbuttons and naxes fields of the joystick structure.
   It returns 0, or -1 if there is an error.
 */
int SDL_SYS_JoystickOpen(SDL_Joystick *joystick)
{
	joystick->naxes = NSP_NUMAXES;
	joystick->nbuttons = NSP_NUMBUTTONS;
	joystick->nhats = 0;
	joystick->nballs = 0;
	return(0);
}

/* Function to update the state of a joystick - called as a device poll.
 * This function shouldn't update the joystick structure directly,
 * but instead should call SDL_PrivateJoystick*() to deliver events
 * and update joystick device state.
 */
void SDL_SYS_JoystickUpdate(SDL_Joystick *joystick)
{
	int i;

	/* Update axes */
	for ( i = 0; i < NSP_NUMAXES; ++i ) {
		t_key pos_key = (i == NSP_JA_H) ? KEY_NSPIRE_RIGHT : KEY_NSPIRE_DOWN;
		t_key neg_key = (i == NSP_JA_H) ? KEY_NSPIRE_LEFT : KEY_NSPIRE_UP;
		BOOL pos_key_pressed = isKeyPressed(pos_key);
		BOOL neg_key_pressed = isKeyPressed(neg_key);
		if ( ja_state[i] == SDL_RELEASED ) {
			if ( pos_key_pressed ) {
				SDL_PrivateJoystickAxis(joystick, i, NSP_JOYAXISVALUE);
				ja_state[i] = SDL_PRESSED;
			} else if ( neg_key_pressed ) {
				SDL_PrivateJoystickAxis(joystick, i, -NSP_JOYAXISVALUE);
				ja_state[i] = SDL_PRESSED;
			}
		} else if ( ! ( pos_key_pressed || neg_key_pressed ) )
			ja_state[i] = SDL_RELEASED;
	}

	/* Update buttons */
	for ( i = 0; i < NSP_NUMBUTTONS; ++i ) {
		BOOL key_pressed = isKeyPressed(js_keymap[i]);
		if ( jb_state[i] == SDL_RELEASED ) {
			if ( key_pressed ) {
				SDL_PrivateJoystickButton(joystick, i, SDL_PRESSED);
				jb_state[i] = SDL_PRESSED;
			}
		} else if ( ! key_pressed ) {
			SDL_PrivateJoystickButton(joystick, i, SDL_RELEASED);
			jb_state[i] = SDL_RELEASED;
		}
	}
}

/* Function to close a joystick after use */
void SDL_SYS_JoystickClose(SDL_Joystick *joystick)
{
	return;
}

/* Function to perform any system-specific joystick related cleanup */
void SDL_SYS_JoystickQuit(void)
{
	return;
}

#endif /* SDL_JOYSTICK_TINSPIRE */
