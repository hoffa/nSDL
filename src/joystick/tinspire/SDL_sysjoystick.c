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

#define NSP_UPDATE_JB(button, jb) if(isKeyPressed((button))) SDL_PrivateJoystickButton(joystick, jb, SDL_PRESSED); \
								  else SDL_PrivateJoystickButton(joystick, jb, SDL_RELEASED)

/* Joystick axes */
enum {
	NSP_JA_H = 0,
	NSP_JA_V,
	NSP_JA_NUMAXES
};

/* Joystick buttons */
enum {
	NSP_JB_ESC = 0,
	NSP_JB_SCRATCHPAD,
	NSP_JB_TAB,
	NSP_JB_HOME,
	NSP_JB_DOC,
	NSP_JB_MENU,
	NSP_JB_CTRL,
	NSP_JB_SHIFT,
	NSP_JB_VAR,
	NSP_JB_DEL,
	NSP_JB_0,
	NSP_JB_1,
	NSP_JB_2,
	NSP_JB_3,
	NSP_JB_4,
	NSP_JB_5,
	NSP_JB_6,
	NSP_JB_7,
	NSP_JB_8,
	NSP_JB_9,
	NSP_JB_PERIOD,
	NSP_JB_NEGATIVE,
	NSP_JB_ENTER,
	NSP_JB_CLICK,
	NSP_JB_NUMBUTTONS
};

/* Function to scan the system for joysticks.
 * This function should set SDL_numjoysticks to the number of available
 * joysticks.  Joystick 0 should be the system default joystick.
 * It should return 0, or -1 on an unrecoverable fatal error.
 */
int SDL_SYS_JoystickInit(void)
{
	SDL_numjoysticks = 1;
	return(0);
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
	joystick->naxes = NSP_JA_NUMAXES;
	joystick->nbuttons = NSP_JB_NUMBUTTONS;
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
	NSP_UPDATE_JB(KEY_NSPIRE_ESC, NSP_JB_ESC);
	NSP_UPDATE_JB(KEY_NSPIRE_SCRATCHPAD, NSP_JB_SCRATCHPAD);
	NSP_UPDATE_JB(KEY_NSPIRE_TAB, NSP_JB_TAB);
	NSP_UPDATE_JB(KEY_NSPIRE_HOME, NSP_JB_HOME);
	NSP_UPDATE_JB(KEY_NSPIRE_DOC, NSP_JB_DOC);
	NSP_UPDATE_JB(KEY_NSPIRE_MENU, NSP_JB_MENU);
	NSP_UPDATE_JB(KEY_NSPIRE_CTRL, NSP_JB_CTRL);
	NSP_UPDATE_JB(KEY_NSPIRE_SHIFT, NSP_JB_SHIFT);
	NSP_UPDATE_JB(KEY_NSPIRE_VAR, NSP_JB_VAR);
	NSP_UPDATE_JB(KEY_NSPIRE_DEL, NSP_JB_DEL);
	NSP_UPDATE_JB(KEY_NSPIRE_0, NSP_JB_0);
	NSP_UPDATE_JB(KEY_NSPIRE_1, NSP_JB_1);
	NSP_UPDATE_JB(KEY_NSPIRE_2, NSP_JB_2);
	NSP_UPDATE_JB(KEY_NSPIRE_3, NSP_JB_3);
	NSP_UPDATE_JB(KEY_NSPIRE_4, NSP_JB_4);
	NSP_UPDATE_JB(KEY_NSPIRE_5, NSP_JB_5);
	NSP_UPDATE_JB(KEY_NSPIRE_6, NSP_JB_6);
	NSP_UPDATE_JB(KEY_NSPIRE_7, NSP_JB_7);
	NSP_UPDATE_JB(KEY_NSPIRE_8, NSP_JB_8);
	NSP_UPDATE_JB(KEY_NSPIRE_9, NSP_JB_9);
	NSP_UPDATE_JB(KEY_NSPIRE_PERIOD, NSP_JB_PERIOD);
	NSP_UPDATE_JB(KEY_NSPIRE_NEGATIVE, NSP_JB_NEGATIVE);
	NSP_UPDATE_JB(KEY_NSPIRE_ENTER, NSP_JB_ENTER);
	NSP_UPDATE_JB(KEY_NSPIRE_CLICK, NSP_JB_CLICK);
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
