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

#include "SDL_tinspirevideo.h"

#define NSP_UPDATE_KEY_EVENT(s, sc, ks, kp) do { \
	SDL_keysym keysym; \
	keysym.scancode = sc; \
	keysym.sym = s; \
	if ( ks == SDL_RELEASED ) { \
		if ( kp ) { \
			SDL_PrivateKeyboard(SDL_PRESSED, &keysym); \
			ks = SDL_PRESSED; \
		} \
	} else if ( ! kp ) { \
		SDL_PrivateKeyboard(SDL_RELEASED, &keysym); \
		ks = SDL_RELEASED; \
	} \
} while (0)

/* Keys */
enum {
	NSP_KEY_A,
	NSP_KEY_B,
	NSP_KEY_C,
	NSP_KEY_D,
	NSP_KEY_E,
	NSP_KEY_F,
	NSP_KEY_G,
	NSP_KEY_H,
	NSP_KEY_I,
	NSP_KEY_J,
	NSP_KEY_K,
	NSP_KEY_L,
	NSP_KEY_M,
	NSP_KEY_N,
	NSP_KEY_O,
	NSP_KEY_P,
	NSP_KEY_Q,
	NSP_KEY_R,
	NSP_KEY_S,
	NSP_KEY_T,
	NSP_KEY_U,
	NSP_KEY_V,
	NSP_KEY_W,
	NSP_KEY_X,
	NSP_KEY_Y,
	NSP_KEY_Z,
	NSP_KEY_0,
	NSP_KEY_1,
	NSP_KEY_2,
	NSP_KEY_3,
	NSP_KEY_4,
	NSP_KEY_5,
	NSP_KEY_6,
	NSP_KEY_7,
	NSP_KEY_8,
	NSP_KEY_9,
	NSP_KEY_RET,
	NSP_KEY_ENTER,
	NSP_KEY_SPACE,
	NSP_KEY_NEGATIVE,
	NSP_KEY_PERIOD,
	NSP_KEY_COMMA,
	NSP_KEY_PLUS,
	NSP_KEY_MINUS,
	NSP_KEY_COLON,
	NSP_KEY_MULTIPLY,
	NSP_KEY_QUOTE,
	NSP_KEY_DIVIDE,
	NSP_KEY_APOSTROPHE,
	NSP_KEY_RP,
	NSP_KEY_LP,
	NSP_KEY_CAPS,
	NSP_KEY_DEL,
	NSP_KEY_ESC,
	NSP_KEY_TAB,
	NSP_KEY_EQU,
	NSP_KEY_SHIFT,
	NSP_KEY_CTRL,
	NSP_KEY_BAR,
	NSP_KEY_VAR,
	NSP_KEY_DOC,
	NSP_KEY_TRIG,
	NSP_KEY_THETA,
	NSP_KEY_LTHAN,
	NSP_KEY_FLAG,
	NSP_KEY_CLICK,
	NSP_KEY_HOME,
	NSP_KEY_MENU,
	NSP_KEY_TAN,
	NSP_KEY_COS,
	NSP_KEY_SIN,
	NSP_KEY_EXP,
	NSP_KEY_GTHAN,
	NSP_KEY_SQU,
	NSP_KEY_TENX,
	NSP_KEY_EE,
	NSP_KEY_II,
	NSP_KEY_eEXP,
	NSP_KEY_PI,
	NSP_KEY_QUES,
	NSP_KEY_QUESEXCL,
	NSP_KEY_CAT,
	NSP_KEY_FRAC,
	NSP_KEY_SCRATCHPAD,
	NSP_NUMKEYS
};

/* Variables and functions exported by SDL_sysevents.c to other parts
   of the native video subsystem (SDL_sysvideo.c)
*/
extern void NSP_InitOSKeymap(_THIS);
extern void NSP_PumpEvents(_THIS);

/* end of SDL_tinspireevents_c.h ... */