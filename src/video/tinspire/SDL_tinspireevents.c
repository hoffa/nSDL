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
#include "../SDL_cursor_c.h"

#include "SDL_tinspirevideo.h"
#include "SDL_tinspireevents_c.h"

static t_key nspk_keymap[NSP_NUMKEYS];
static SDLKey sdlk_keymap[NSP_NUMKEYS] = {SDLK_UNKNOWN};
static char key_state[NSP_NUMKEYS] = {SDL_RELEASED};
static char mousebutton_state = SDL_RELEASED;

void NSP_PumpEvents(_THIS)
{
	Sint16 dx_sum, dy_sum;
	int i;

	for ( i = dx_sum = dy_sum = 0; i < NSP_NUMKEYS; ++i ) {
		BOOL key_pressed = isKeyPressed(nspk_keymap[i]);
		SDL_keysym keysym;
		nsp_tp_t tp;

		if ( is_touchpad
		  && touchpad_read(0x06, 0x07, &tp)
		  && ( tp.dx || tp.dy ) ) {
			dx_sum += tp.dx;
			dy_sum += -tp.dy;
		}

		keysym.scancode = i;
		keysym.sym = sdlk_keymap[i];
		if ( key_state[i] == SDL_RELEASED ) {
			if ( key_pressed ) {
				SDL_PrivateKeyboard(SDL_PRESSED, &keysym);
				key_state[i] = SDL_PRESSED;
			}
		} else if ( ! key_pressed ) {
			SDL_PrivateKeyboard(SDL_RELEASED, &keysym);
			key_state[i] = SDL_RELEASED;
		}
	}

	if ( this->hidden->use_mouse ) {
		BOOL mousebutton_pressed = isKeyPressed(KEY_NSPIRE_CLICK);
		if ( mousebutton_state == SDL_RELEASED ) {
			if ( mousebutton_pressed ) {
				SDL_PrivateMouseButton(SDL_PRESSED, 0, 0, 0);
				mousebutton_state = SDL_PRESSED;
			}
		} else if ( ! mousebutton_pressed ) {
			SDL_PrivateMouseButton(SDL_RELEASED, 0, 0, 0);
			mousebutton_state = SDL_RELEASED;
		}

		/* We don't want to draw the cursor right away,
		   but at the very last moment, when calling SDL_UpdateRects() */
		SDL_cursorstate &= ~CURSOR_USINGSW;

		if ( dx_sum || dy_sum )
			SDL_PrivateMouseMotion(0, SDL_TRUE, dx_sum, dy_sum);
	}

}

void NSP_InitOSKeymap(_THIS)
{
	/* Enum value -> KEY_NSPIRE_* */
	nspk_keymap[NSP_KEY_RET] =	KEY_NSPIRE_RET;
	nspk_keymap[NSP_KEY_ENTER] =	KEY_NSPIRE_ENTER;
	nspk_keymap[NSP_KEY_SPACE] =	KEY_NSPIRE_SPACE;
	nspk_keymap[NSP_KEY_NEGATIVE] =	KEY_NSPIRE_NEGATIVE;
	nspk_keymap[NSP_KEY_Z] =	KEY_NSPIRE_Z;
	nspk_keymap[NSP_KEY_PERIOD] =	KEY_NSPIRE_PERIOD;
	nspk_keymap[NSP_KEY_Y] =	KEY_NSPIRE_Y;
	nspk_keymap[NSP_KEY_0] =	KEY_NSPIRE_0;
	nspk_keymap[NSP_KEY_X] =	KEY_NSPIRE_X;
	nspk_keymap[NSP_KEY_THETA] =	KEY_NSPIRE_THETA;
	nspk_keymap[NSP_KEY_COMMA] =	KEY_NSPIRE_COMMA;
	nspk_keymap[NSP_KEY_PLUS] =	KEY_NSPIRE_PLUS;
	nspk_keymap[NSP_KEY_W] =	KEY_NSPIRE_W;
	nspk_keymap[NSP_KEY_3] =	KEY_NSPIRE_3;
	nspk_keymap[NSP_KEY_V] =	KEY_NSPIRE_V;
	nspk_keymap[NSP_KEY_2] =	KEY_NSPIRE_2;
	nspk_keymap[NSP_KEY_U] =	KEY_NSPIRE_U;
	nspk_keymap[NSP_KEY_1] =	KEY_NSPIRE_1;
	nspk_keymap[NSP_KEY_T] =	KEY_NSPIRE_T;
	nspk_keymap[NSP_KEY_eEXP] =	KEY_NSPIRE_eEXP;
	nspk_keymap[NSP_KEY_PI] =	KEY_NSPIRE_PI;
	nspk_keymap[NSP_KEY_QUES] =	KEY_NSPIRE_QUES;
	nspk_keymap[NSP_KEY_QUESEXCL] =	KEY_NSPIRE_QUESEXCL;
	nspk_keymap[NSP_KEY_MINUS] =	KEY_NSPIRE_MINUS;
	nspk_keymap[NSP_KEY_S] =	KEY_NSPIRE_S;
	nspk_keymap[NSP_KEY_6] =	KEY_NSPIRE_6;
	nspk_keymap[NSP_KEY_R] =	KEY_NSPIRE_R;
	nspk_keymap[NSP_KEY_5] =	KEY_NSPIRE_5;
	nspk_keymap[NSP_KEY_Q] =	KEY_NSPIRE_Q;
	nspk_keymap[NSP_KEY_4] =	KEY_NSPIRE_4;
	nspk_keymap[NSP_KEY_P] =	KEY_NSPIRE_P;
	nspk_keymap[NSP_KEY_TENX] =	KEY_NSPIRE_TENX;
	nspk_keymap[NSP_KEY_EE] =	KEY_NSPIRE_EE;
	nspk_keymap[NSP_KEY_COLON] =	KEY_NSPIRE_COLON;
	nspk_keymap[NSP_KEY_MULTIPLY] =	KEY_NSPIRE_MULTIPLY;
	nspk_keymap[NSP_KEY_O] =	KEY_NSPIRE_O;
	nspk_keymap[NSP_KEY_9] =	KEY_NSPIRE_9;
	nspk_keymap[NSP_KEY_N] =	KEY_NSPIRE_N;
	nspk_keymap[NSP_KEY_8] =	KEY_NSPIRE_8;
	nspk_keymap[NSP_KEY_M] =	KEY_NSPIRE_M;
	nspk_keymap[NSP_KEY_7] =	KEY_NSPIRE_7;
	nspk_keymap[NSP_KEY_L] =	KEY_NSPIRE_L;
	nspk_keymap[NSP_KEY_SQU] =	KEY_NSPIRE_SQU;
	nspk_keymap[NSP_KEY_II] =	KEY_NSPIRE_II;
	nspk_keymap[NSP_KEY_QUOTE] =	KEY_NSPIRE_QUOTE;
	nspk_keymap[NSP_KEY_DIVIDE] =	KEY_NSPIRE_DIVIDE;
	nspk_keymap[NSP_KEY_K] =	KEY_NSPIRE_K;
	nspk_keymap[NSP_KEY_TAN] =	KEY_NSPIRE_TAN;
	nspk_keymap[NSP_KEY_J] =	KEY_NSPIRE_J;
	nspk_keymap[NSP_KEY_COS] =	KEY_NSPIRE_COS;
	nspk_keymap[NSP_KEY_I] =	KEY_NSPIRE_I;
	nspk_keymap[NSP_KEY_SIN] =	KEY_NSPIRE_SIN;
	nspk_keymap[NSP_KEY_H] =	KEY_NSPIRE_H;
	nspk_keymap[NSP_KEY_EXP] =	KEY_NSPIRE_EXP;
	nspk_keymap[NSP_KEY_GTHAN] =	KEY_NSPIRE_GTHAN;
	nspk_keymap[NSP_KEY_APOSTROPHE] = KEY_NSPIRE_APOSTROPHE;
	nspk_keymap[NSP_KEY_CAT] =	KEY_NSPIRE_CAT;
	nspk_keymap[NSP_KEY_FRAC] =	KEY_NSPIRE_FRAC;
	nspk_keymap[NSP_KEY_G] =	KEY_NSPIRE_G;
	nspk_keymap[NSP_KEY_RP] =	KEY_NSPIRE_RP;
	nspk_keymap[NSP_KEY_F] =	KEY_NSPIRE_F;
	nspk_keymap[NSP_KEY_LP] =	KEY_NSPIRE_LP;
	nspk_keymap[NSP_KEY_E] =	KEY_NSPIRE_E;
	nspk_keymap[NSP_KEY_VAR] =	KEY_NSPIRE_VAR;
	nspk_keymap[NSP_KEY_D] =	KEY_NSPIRE_D;
	nspk_keymap[NSP_KEY_CAPS] =	KEY_NSPIRE_CAPS;
	nspk_keymap[NSP_KEY_DEL] =	KEY_NSPIRE_DEL;
	nspk_keymap[NSP_KEY_LTHAN] =	KEY_NSPIRE_LTHAN;
	nspk_keymap[NSP_KEY_FLAG] =	KEY_NSPIRE_FLAG;
	nspk_keymap[NSP_KEY_C] =	KEY_NSPIRE_C;
	nspk_keymap[NSP_KEY_HOME] =	KEY_NSPIRE_HOME;
	nspk_keymap[NSP_KEY_B] =	KEY_NSPIRE_B;
	nspk_keymap[NSP_KEY_MENU] =	KEY_NSPIRE_MENU;
	nspk_keymap[NSP_KEY_A] =	KEY_NSPIRE_A;
	nspk_keymap[NSP_KEY_ESC] =	KEY_NSPIRE_ESC;
	nspk_keymap[NSP_KEY_BAR] =	KEY_NSPIRE_BAR;
	nspk_keymap[NSP_KEY_TAB] =	KEY_NSPIRE_TAB;
	nspk_keymap[NSP_KEY_EQU] =	KEY_NSPIRE_EQU;
	nspk_keymap[NSP_KEY_UP] =	KEY_NSPIRE_UP;
	nspk_keymap[NSP_KEY_UPRIGHT] =	KEY_NSPIRE_UPRIGHT;
	nspk_keymap[NSP_KEY_RIGHT] =	KEY_NSPIRE_RIGHT;
	nspk_keymap[NSP_KEY_RIGHTDOWN] = KEY_NSPIRE_RIGHTDOWN;
	nspk_keymap[NSP_KEY_DOWN] =	KEY_NSPIRE_DOWN;
	nspk_keymap[NSP_KEY_DOWNLEFT] =	KEY_NSPIRE_DOWNLEFT;
	nspk_keymap[NSP_KEY_LEFT] =	KEY_NSPIRE_LEFT;
	nspk_keymap[NSP_KEY_LEFTUP] =	KEY_NSPIRE_LEFTUP;
	nspk_keymap[NSP_KEY_SHIFT] =	KEY_NSPIRE_SHIFT;
	nspk_keymap[NSP_KEY_CTRL] =	KEY_NSPIRE_CTRL;
	nspk_keymap[NSP_KEY_DOC] =	KEY_NSPIRE_DOC;
	nspk_keymap[NSP_KEY_TRIG] =	KEY_NSPIRE_TRIG;
	nspk_keymap[NSP_KEY_SCRATCHPAD] = KEY_NSPIRE_SCRATCHPAD;

	/* Enum value -> SDLK_* */
	sdlk_keymap[NSP_KEY_A] =	SDLK_a;
	sdlk_keymap[NSP_KEY_B] =	SDLK_b;
	sdlk_keymap[NSP_KEY_C] =	SDLK_c;
	sdlk_keymap[NSP_KEY_D] =	SDLK_d;
	sdlk_keymap[NSP_KEY_E] =	SDLK_e;
	sdlk_keymap[NSP_KEY_F] =	SDLK_f;
	sdlk_keymap[NSP_KEY_G] =	SDLK_g;
	sdlk_keymap[NSP_KEY_H] =	SDLK_h;
	sdlk_keymap[NSP_KEY_I] =	SDLK_i;
	sdlk_keymap[NSP_KEY_J] =	SDLK_j;
	sdlk_keymap[NSP_KEY_K] =	SDLK_k;
	sdlk_keymap[NSP_KEY_L] =	SDLK_l;
	sdlk_keymap[NSP_KEY_M] =	SDLK_m;
	sdlk_keymap[NSP_KEY_N] =	SDLK_n;
	sdlk_keymap[NSP_KEY_O] =	SDLK_o;
	sdlk_keymap[NSP_KEY_P] =	SDLK_p;
	sdlk_keymap[NSP_KEY_Q] =	SDLK_q;
	sdlk_keymap[NSP_KEY_R] =	SDLK_r;
	sdlk_keymap[NSP_KEY_S] =	SDLK_s;
	sdlk_keymap[NSP_KEY_T] =	SDLK_t;
	sdlk_keymap[NSP_KEY_U] =	SDLK_u;
	sdlk_keymap[NSP_KEY_V] =	SDLK_v;
	sdlk_keymap[NSP_KEY_W] =	SDLK_w;
	sdlk_keymap[NSP_KEY_X] =	SDLK_x;
	sdlk_keymap[NSP_KEY_Y] =	SDLK_y;
	sdlk_keymap[NSP_KEY_Z] =	SDLK_z;
	sdlk_keymap[NSP_KEY_0] =	SDLK_0;
	sdlk_keymap[NSP_KEY_1] =	SDLK_1;
	sdlk_keymap[NSP_KEY_2] =	SDLK_2;
	sdlk_keymap[NSP_KEY_3] =	SDLK_3;
	sdlk_keymap[NSP_KEY_4] =	SDLK_4;
	sdlk_keymap[NSP_KEY_5] =	SDLK_5;
	sdlk_keymap[NSP_KEY_6] =	SDLK_6;
	sdlk_keymap[NSP_KEY_7] =	SDLK_7;
	sdlk_keymap[NSP_KEY_8] =	SDLK_8;
	sdlk_keymap[NSP_KEY_9] =	SDLK_9;
	sdlk_keymap[NSP_KEY_RET] =	SDLK_RETURN;
	sdlk_keymap[NSP_KEY_ENTER] =	SDLK_RETURN;
	sdlk_keymap[NSP_KEY_SPACE] =	SDLK_SPACE;
	sdlk_keymap[NSP_KEY_NEGATIVE] =	SDLK_MINUS;
	sdlk_keymap[NSP_KEY_PERIOD] =	SDLK_PERIOD;
	sdlk_keymap[NSP_KEY_COMMA] =	SDLK_COMMA;
	sdlk_keymap[NSP_KEY_PLUS] =	SDLK_PLUS;
	sdlk_keymap[NSP_KEY_MINUS] =	SDLK_MINUS;
	sdlk_keymap[NSP_KEY_COLON] =	SDLK_COLON;
	sdlk_keymap[NSP_KEY_MULTIPLY] =	SDLK_ASTERISK;
	sdlk_keymap[NSP_KEY_QUOTE] =	SDLK_QUOTEDBL;
	sdlk_keymap[NSP_KEY_DIVIDE] =	SDLK_SLASH;
	sdlk_keymap[NSP_KEY_APOSTROPHE] = SDLK_QUOTE;
	sdlk_keymap[NSP_KEY_RP] =	SDLK_RIGHTPAREN;
	sdlk_keymap[NSP_KEY_LP] =	SDLK_LEFTPAREN;
	sdlk_keymap[NSP_KEY_CAPS] =	SDLK_CAPSLOCK;
	sdlk_keymap[NSP_KEY_DEL] =	SDLK_BACKSPACE;
	sdlk_keymap[NSP_KEY_ESC] =	SDLK_ESCAPE;
	sdlk_keymap[NSP_KEY_TAB] =	SDLK_TAB;
	sdlk_keymap[NSP_KEY_EQU] =	SDLK_EQUALS;
	sdlk_keymap[NSP_KEY_UP] =	SDLK_UP;
	sdlk_keymap[NSP_KEY_RIGHT] =	SDLK_RIGHT;
	sdlk_keymap[NSP_KEY_DOWN] =	SDLK_DOWN;
	sdlk_keymap[NSP_KEY_LEFT] =	SDLK_LEFT;
	sdlk_keymap[NSP_KEY_SHIFT] =	SDLK_LSHIFT;
	sdlk_keymap[NSP_KEY_CTRL] =	SDLK_LCTRL;
	sdlk_keymap[NSP_KEY_UPRIGHT] =	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_RIGHTDOWN] = SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_DOWNLEFT] =	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_LEFTUP] =	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_BAR] =	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_VAR] =	SDLK_LALT;
	sdlk_keymap[NSP_KEY_DOC] =	SDLK_LMETA;
	sdlk_keymap[NSP_KEY_TRIG] =	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_THETA] =	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_LTHAN] =	SDLK_LESS;
	sdlk_keymap[NSP_KEY_FLAG] =	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_HOME] = 	SDLK_HOME;
	sdlk_keymap[NSP_KEY_MENU] =	SDLK_RALT;
	sdlk_keymap[NSP_KEY_TAN] =	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_COS] =	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_SIN] =	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_EXP] =	SDLK_CARET;
	sdlk_keymap[NSP_KEY_GTHAN] =	SDLK_GREATER;
	sdlk_keymap[NSP_KEY_SQU] =	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_TENX]=	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_EE] =	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_II] =	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_eEXP] =	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_PI] =	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_QUES] =	SDLK_QUESTION;
	sdlk_keymap[NSP_KEY_QUESEXCL] =	SDLK_EXCLAIM;
	sdlk_keymap[NSP_KEY_CAT] =	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_FRAC] =	SDLK_UNKNOWN;
	sdlk_keymap[NSP_KEY_SCRATCHPAD] = SDLK_LSUPER;
}

/* end of SDL_tinspireevents.c ... */

