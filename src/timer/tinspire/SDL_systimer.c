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

#include <os.h>
#include "SDL_config.h"

#ifdef SDL_TIMER_TINSPIRE

#include "SDL_timer.h"
#include "../SDL_timer_c.h"

static volatile unsigned *value;
static volatile unsigned *control;
Uint32 tick_sum = 0;
Uint32 start = 0;

/*
Uses the first timer (0x900C0000)
TC: http://hackspire.unsads.com/wiki/index.php/Memory-mapped_I/O_ports#900C0000_-_First_timer
CX: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0271d/Babehiha.html  
*/

void SDL_StartTicks(void)
{
	*(volatile unsigned *)0x900B0018 &= ~(1 << 11); /* Enable bus access */
	if ( is_cx ) {
		value = (unsigned *)0x900C0004;
		control = (unsigned *)0x900C0008;
		*(volatile unsigned *)0x900C0080 = 0xA; /* "[...] on the CX to specify the 32768Hz clock as the source for the First Timer" */
		*control = 0b10000010; /* Wrapping; 32-bit; divider to 1; interrupt disabled; free-running; start */
		start = *value;
	} else {
		value = (unsigned *)0x900C0000;
		control = (unsigned *)0x900C0008;
		*(volatile unsigned *)0x900C0004 = 33; /* Set time divider to 33; 32768/33=993Hz (approx. 1000Hz) */
		*control = 0b00001111; /* Run infinitely; increasing; start */
		*value = 0;
	}
}

Uint32 SDL_GetTicks (void)
{
	if ( is_cx )
		return((start - *value) / 33);
	else {
		tick_sum += *value;
		*value = 0;
		return(tick_sum);
	}
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
