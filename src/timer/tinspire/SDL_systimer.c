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

/* TODO: just fix this shit, add for CX models and whatnot */

/* Timer should be set to 1024 Hz */
#define NSP_RTC_GET_VALUE()	NSP_ADDRVAL(0x90090000)
#if !NSP_COLOR_LCD
#define NSP_TIMER_VALUE			NSP_ADDRVAL(0x900c000c)
#define NSP_TIMER_GET_VALUE()		NSP_TIMER_VALUE
#define NSP_TIMER_SET_VALUE(value)	NSP_TIMER_VALUE = value
#define NSP_TIMER_START()	NSP_ADDRVAL(0x900c0014) = 0xf
#define NSP_TIMER_STOP()	NSP_ADDRVAL(0x900c0014) = 0x1f
#define NSP_TIMER_RESET()	NSP_TIMER_SET_VALUE(0); \
    				NSP_ADDRVAL(0x900c0010) = 0; \
    				NSP_TIMER_STOP()
#define NSP_TIMER_INIT()	NSP_ADDRVAL(0x900b0018) &= ~(1 << 11); \
    				NSP_ADDRVAL(0x900c0010) = 32
#endif

Uint32 cur_time = 0;
Uint32 prev_rtc;

void SDL_StartTicks(void)
{
	NSP_TIMER_RESET();
	NSP_TIMER_INIT();
	NSP_TIMER_SET_VALUE(0);
	NSP_TIMER_START();
	prev_rtc = NSP_RTC_GET_VALUE();
}

Uint32 SDL_GetTicks (void)
{
	Uint32 cur_rtc = NSP_RTC_GET_VALUE();
	Uint32 delta_rtc = cur_rtc - prev_rtc;
	cur_time += NSP_TIMER_GET_VALUE();
	if ( delta_rtc >= 64 ) /* It takes 64 seconds before the timer resets */
		cur_time += (delta_rtc >> 6) << 16;
	prev_rtc = cur_rtc;
	NSP_TIMER_SET_VALUE(0);
	return(cur_time);
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
