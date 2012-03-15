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

#ifdef SDL_TIMER_TINSPIRE

#include "SDL_timer.h"
#include "../SDL_timer_c.h"

#if NSP_COLOR_LCD
static volatile unsigned *value = (unsigned *)0x900C0004;
static volatile unsigned *control = (unsigned *)0x900C0008;
#else
static volatile unsigned *value = (unsigned *)0x900C000C;
static volatile unsigned *control = (unsigned *)0x900C0014;
Uint32 tick_sum = 0;
#endif

Uint32 start = 0;

void SDL_StartTicks(void)
{
	*(volatile unsigned *)0x900B0018 &= ~(1 << 11);
	*(volatile unsigned *)0x900C0080 = 0xA;
#if NSP_COLOR_LCD
	*control = 0b10100110;
	start = *value;
#else
	*control = 0b10000;
	*(volatile unsigned *)0x900C0010 = 32;
	*value = 0;
	*control = 0b01111;
#endif
}

Uint32 SDL_GetTicks (void)
{
#if NSP_COLOR_LCD
	return((start - *value) >> 1);
#else
	tick_sum += *value;
	*value = 0;
	return(tick_sum);
#endif
}

void SDL_Delay (Uint32 ms)
{
	sleep(ms);
}

#include "SDL_thread.h"

/* Data to handle a single periodic alarm */
static int timer_alive = 0;
static SDL_Thread *timer = NULL;

static int RunTimer(void *unused)
{
	while ( timer_alive ) {
		if ( SDL_timer_running ) {
			SDL_ThreadedTimerCheck();
		}
		SDL_Delay(1);
	}
	return(0);
}

/* This is only called if the event thread is not running */
int SDL_SYS_TimerInit(void)
{
	timer_alive = 1;
	timer = SDL_CreateThread(RunTimer, NULL);
	if ( timer == NULL )
		return(-1);
	return(SDL_SetTimerThreaded(1));
}

void SDL_SYS_TimerQuit(void)
{
	timer_alive = 0;
	if ( timer ) {
		SDL_WaitThread(timer, NULL);
		timer = NULL;
	}
}

int SDL_SYS_StartTimer(void)
{
	SDL_SetError("Timers not implemented on the TI-Nspire");
	return(-1);
}

void SDL_SYS_StopTimer(void)
{
	return;
}

#endif /* SDL_TIMER_TINSPIRE */
