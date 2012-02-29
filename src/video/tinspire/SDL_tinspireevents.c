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

#include "SDL.h"
#include "../../events/SDL_sysevents.h"
#include "../../events/SDL_events_c.h"

#include "SDL_tinspirevideo.h"
#include "SDL_tinspireevents_c.h"

static SDLKey keymap[NSP_MAX_KEYVAL];

void NSP_PumpEvents(_THIS)
{
	/* do nothing. */
}

/* stub; maps nspire key to sdlkey */
void NSP_InitOSKeymap(_THIS)
{
/*
	keymap[KEY_NSPIRE_RET] = 		SDLK_RETURN;
	keymap[KEY_NSPIRE_ENTER] = 		SDLK_RETURN;
	keymap[KEY_NSPIRE_SPACE] = 		SDLK_SPACE;
	keymap[KEY_NSPIRE_NEGATIVE] =	SDLK_MINUS;
	keymap[KEY_NSPIRE_Z] = 			SDLK_z;
	keymap[KEY_NSPIRE_PERIOD] = 	SDLK_PERIOD;
	keymap[KEY_NSPIRE_Y] = 			SDLK_y;
	keymap[KEY_NSPIRE_0] = 			SDLK_0;
	keymap[KEY_NSPIRE_X] = 			SDLK_x;
	keymap[KEY_NSPIRE_THETA] =
	keymap[KEY_NSPIRE_COMMA] = 		SDLK_COMMA;
	keymap[KEY_NSPIRE_PLUS] = 		SDLK_PLUS;
	keymap[KEY_NSPIRE_W] = 			SDLK_w;
	keymap[KEY_NSPIRE_3] = 			SDLK_3;
	keymap[KEY_NSPIRE_V] = 			SDLK_v;
	keymap[KEY_NSPIRE_2] = 			SDLK_2;
	keymap[KEY_NSPIRE_U] = 			SDLK_u;
	keymap[KEY_NSPIRE_1] = 			SDLK_1;
	keymap[KEY_NSPIRE_T] = 			SDLK_t;
	keymap[KEY_NSPIRE_eEXP] =
	keymap[KEY_NSPIRE_PI] =
	keymap[KEY_NSPIRE_QUES] =
	keymap[KEY_NSPIRE_QUESEXCL] =
	keymap[KEY_NSPIRE_MINUS] = 		SDLK_MINUS;
	keymap[KEY_NSPIRE_S] = 			SDLK_s;
	keymap[KEY_NSPIRE_6] = 			SDLK_6;
	keymap[KEY_NSPIRE_R] = 			SDLK_r;
	keymap[KEY_NSPIRE_5] = 			SDLK_5;
	keymap[KEY_NSPIRE_Q] = 			SDLK_q;
	keymap[KEY_NSPIRE_4] = 			SDLK_4;
	keymap[KEY_NSPIRE_P] = 			SDLK_p;
	keymap[KEY_NSPIRE_TENX] =
	keymap[KEY_NSPIRE_EE] =
	keymap[KEY_NSPIRE_COLON] = 		SDLK_COLON;
	keymap[KEY_NSPIRE_MULTIPLY] =	SDLK_ASTERISK;
	keymap[KEY_NSPIRE_O] = 			SDLK_o;
	keymap[KEY_NSPIRE_9] = 			SDLK_9;
	keymap[KEY_NSPIRE_N] = 			SDLK_n;
	keymap[KEY_NSPIRE_8] = 			SDLK_8;
	keymap[KEY_NSPIRE_M] = 			SDLK_m;
	keymap[KEY_NSPIRE_7] = 			SDLK_7;
	keymap[KEY_NSPIRE_L] = 			SDLK_l;
	keymap[KEY_NSPIRE_SQU] =
	keymap[KEY_NSPIRE_II] =
	keymap[KEY_NSPIRE_QUOTE] = 		SDLK_QUOTEDBL;
	keymap[KEY_NSPIRE_DIVIDE] =		SDLK_SLASH;
	keymap[KEY_NSPIRE_K] = 			SDLK_k;
	keymap[KEY_NSPIRE_TAN] =
	keymap[KEY_NSPIRE_J] = 			SDLK_j;
	keymap[KEY_NSPIRE_COS] =
	keymap[KEY_NSPIRE_I] = 			SDLK_i;
	keymap[KEY_NSPIRE_SIN] =
	keymap[KEY_NSPIRE_H] = 			SDLK_h;
	keymap[KEY_NSPIRE_EXP] =
	keymap[KEY_NSPIRE_GTHAN] =
	keymap[KEY_NSPIRE_APOSTROPHE] = SDLK_QUOTE;
	keymap[KEY_NSPIRE_CAT] =
	keymap[KEY_NSPIRE_FRAC] =
	keymap[KEY_NSPIRE_G] = 			SDLK_g;
	keymap[KEY_NSPIRE_RP] =			SDLK_RIGHTPAREN;
	keymap[KEY_NSPIRE_F] = 			SDLK_f;
	keymap[KEY_NSPIRE_LP] =			SDLK_LEFTPAREN;
	keymap[KEY_NSPIRE_E] = 			SDLK_e;
	keymap[KEY_NSPIRE_VAR] =
	keymap[KEY_NSPIRE_D] = 			SDLK_d;
	keymap[KEY_NSPIRE_CAPS] = 		SDLK_CAPSLOCK;
	keymap[KEY_NSPIRE_DEL] = 		SDLK_BACKSPACE;
	keymap[KEY_NSPIRE_LTHAN] =
	keymap[KEY_NSPIRE_FLAG] =
	keymap[KEY_NSPIRE_CLICK] =
	keymap[KEY_NSPIRE_C] = 			SDLK_c;
	keymap[KEY_NSPIRE_HOME] = 		SDLK_HOME;
	keymap[KEY_NSPIRE_B] = 			SDLK_b;
	keymap[KEY_NSPIRE_MENU] =
	keymap[KEY_NSPIRE_A] = 			SDLK_a;
	keymap[KEY_NSPIRE_ESC] = 		SDLK_ESCAPE;
	keymap[KEY_NSPIRE_BAR] =
	keymap[KEY_NSPIRE_TAB] = 		SDLK_TAB;
	keymap[KEY_NSPIRE_EQU] =		SDLK_EQUALS;
	keymap[KEY_NSPIRE_UP] = 		SDLK_UP;
	keymap[KEY_NSPIRE_UPRIGHT] =
	keymap[KEY_NSPIRE_RIGHT] = 		SDLK_RIGHT;
	keymap[KEY_NSPIRE_RIGHTDOWN] =
	keymap[KEY_NSPIRE_DOWN] = 		SDLK_DOWN;
	keymap[KEY_NSPIRE_DOWNLEFT] =
	keymap[KEY_NSPIRE_LEFT] = 		SDLK_LEFT;
	keymap[KEY_NSPIRE_LEFTUP] =
	keymap[KEY_NSPIRE_SHIFT] = 		SDLK_LSHIFT;
	keymap[KEY_NSPIRE_CTRL] =       SDLK_LCTRL;
	keymap[KEY_NSPIRE_DOC] =
	keymap[KEY_NSPIRE_TRIG] =
	keymap[KEY_NSPIRE_SCRATCHPAD] =
*/
}

/* end of SDL_tinspireevents.c ... */

