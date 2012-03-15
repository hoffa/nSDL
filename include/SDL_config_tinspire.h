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

#ifndef _SDL_config_tinspire_h
#define _SDL_config_tinspire_h

#include <os.h>

/* General platform specific identifiers */
#include "SDL_platform.h"

/* 1 if CX, 0 if Touchpad/Clickpad */
#define NSP_COLOR_LCD	0

#if 1
#define NSP_DEBUG	1
#define DEBUG_BUILD	1
#define DEBUG_PALETTE	1
#define DEBUG_VIDEO	1
#define DEBUG_ASM	1
#define DEBUG_GRAB	0
#define DEBUG_TIMERS	1
#define DEBUG_QSORT	1
#define DEBUG_THREADS	1
#define DEBUG_ERROR	1
#endif

#define NSP_VERSION	"0.1.0"
#define NSP_JOYAXISVALUE	1

/* A few convenience macros */
#define NSP_ARRAY_SIZE(array)	(sizeof(array) / sizeof(array[0]))
#define NSP_NL_RELOCDATA(ptr, size)	nl_relocdata((unsigned *)(ptr), size)

#if NSP_COLOR_LCD
#define NSP_DBL_IF_CX(n)	(n << 1)
#define NSP_BPP	16
#define NSP_RMASK	0xF800
#define NSP_GMASK	0x7E0
#define NSP_BMASK	0x1F
#else
#define NSP_BPP	8
#define NSP_DBL_IF_CX(n)	(n)
#define NSP_RMASK	0
#define NSP_GMASK	0
#define NSP_BMASK	0
#endif

#if NSP_DEBUG
#define NSP_DPRINT(fmt, args...)	fprintf(stderr, "[NSP] %s():%d: " fmt, __FUNCTION__, __LINE__, ## args)
#else
#define NSP_DPRINT(fmt, ...)
#endif

#define SDL_HAS_64BIT_TYPE	1

/* Endianness */
#define SDL_BYTEORDER	1234

/* Useful headers */
#define HAVE_STDIO_H	1
#define HAVE_STDLIB_H	1
#define HAVE_STDARG_H	1
#define HAVE_STRING_H	1
#define HAVE_CTYPE_H	1
/* #undef HAVE_ALLOCA_H
#undef HAVE_SYS_TYPES_H
#undef STDC_HEADERS
#undef HAVE_MALLOC_H
#undef HAVE_MEMORY_H
#undef HAVE_STRINGS_H
#undef HAVE_INTTYPES_H
#undef HAVE_STDINT_H
#undef HAVE_MATH_H
#undef HAVE_ICONV_H
#undef HAVE_SIGNAL_H
#undef HAVE_ALTIVEC_H */

/* C library functions */
#define HAVE_MALLOC	1
#define HAVE_CALLOC	1
#define HAVE_REALLOC	1
#define HAVE_FREE	1
#define HAVE_MEMSET	1
#define HAVE_MEMCPY	1
#define HAVE_MEMMOVE	1
#define HAVE_MEMCMP	1
#define HAVE_STRLEN	1
#define HAVE_STRCHR	1
#define HAVE_STRSTR	1
#define HAVE_STRTOL	1
#define HAVE_STRTOUL	1
#define HAVE_STRTOD	1
#define HAVE_ATOI	1
#define HAVE_ATOF	1
#define HAVE_STRCMP	1
#define HAVE_STRNCMP	1
/* #undef HAVE_ALLOCA
#ifndef _WIN32
#undef HAVE_GETENV
#undef HAVE_PUTENV
#undef HAVE_UNSETENV
#endif
#undef HAVE_QSORT
#undef HAVE_ABS
#undef HAVE_BCOPY 
#undef HAVE_STRLCPY
#undef HAVE_STRLCAT
#undef HAVE_STRDUP
#undef HAVE__STRREV
#undef HAVE__STRUPR
#undef HAVE__STRLWR
#undef HAVE_INDEX
#undef HAVE_RINDEX
#undef HAVE_STRRCHR
#undef HAVE_ITOA
#undef HAVE__LTOA
#undef HAVE__UITOA
#undef HAVE__ULTOA
#undef HAVE__I64TOA
#undef HAVE__UI64TOA
#undef HAVE_STRTOLL
#undef HAVE_STRTOULL
#undef HAVE__STRICMP
#undef HAVE_STRCASECMP
#undef HAVE__STRNICMP
#undef HAVE_STRNCASECMP
#undef HAVE_SSCANF
#undef HAVE_SNPRINTF
#undef HAVE_VSNPRINTF
#undef HAVE_ICONV
#undef HAVE_SIGACTION
#undef HAVE_SA_SIGACTION
#undef HAVE_SETJMP
#undef HAVE_NANOSLEEP
#undef HAVE_CLOCK_GETTIME
#undef HAVE_GETPAGESIZE
#undef HAVE_MPROTECT
#undef HAVE_SEM_TIMEDWAIT */

/* Allow disabling of core subsystems */
#define SDL_AUDIO_DISABLED	1
#define SDL_CDROM_DISABLED	1
#define SDL_LOADSO_DISABLED	1
#define SDL_THREADS_DISABLED	1

/* Enable various input drivers */
#define SDL_JOYSTICK_TINSPIRE	1

/* Enable various timer systems */
#define SDL_TIMER_TINSPIRE	1

/* Enable various video drivers */
#define SDL_VIDEO_DRIVER_TINSPIRE	1

/* Enable assembly routines */
#define SDL_ASSEMBLY_ROUTINES	1

/* Keys */
enum {
	NSP_KEY_RET = 0,
	NSP_KEY_ENTER,
	NSP_KEY_SPACE,
	NSP_KEY_NEGATIVE,
	NSP_KEY_Z,
	NSP_KEY_PERIOD,
	NSP_KEY_Y,
	NSP_KEY_0,
	NSP_KEY_X,
	NSP_KEY_THETA,
	NSP_KEY_COMMA,
	NSP_KEY_PLUS,
	NSP_KEY_W,
	NSP_KEY_3,
	NSP_KEY_V,
	NSP_KEY_2,
	NSP_KEY_U,
	NSP_KEY_1,
	NSP_KEY_T,
	NSP_KEY_eEXP,
	NSP_KEY_PI,
	NSP_KEY_QUES,
	NSP_KEY_QUESEXCL,
	NSP_KEY_MINUS,
	NSP_KEY_S,
	NSP_KEY_6,
	NSP_KEY_R,
	NSP_KEY_5,
	NSP_KEY_Q,
	NSP_KEY_4,
	NSP_KEY_P,
	NSP_KEY_TENX,
	NSP_KEY_EE,
	NSP_KEY_COLON,
	NSP_KEY_MULTIPLY,
	NSP_KEY_O,
	NSP_KEY_9,
	NSP_KEY_N,
	NSP_KEY_8,
	NSP_KEY_M,
	NSP_KEY_7,
	NSP_KEY_L,
	NSP_KEY_SQU,
	NSP_KEY_II,
	NSP_KEY_QUOTE,
	NSP_KEY_DIVIDE,
	NSP_KEY_K,
	NSP_KEY_TAN,
	NSP_KEY_J,
	NSP_KEY_COS,
	NSP_KEY_I,
	NSP_KEY_SIN,
	NSP_KEY_H,
	NSP_KEY_EXP,
	NSP_KEY_GTHAN,
	NSP_KEY_APOSTROPHE,
	NSP_KEY_CAT,
	NSP_KEY_FRAC,
	NSP_KEY_G,
	NSP_KEY_RP,
	NSP_KEY_F,
	NSP_KEY_LP,
	NSP_KEY_E,
	NSP_KEY_VAR,
	NSP_KEY_D,
	NSP_KEY_CAPS,
	NSP_KEY_DEL,
	NSP_KEY_LTHAN,
	NSP_KEY_FLAG,
	NSP_KEY_CLICK,
	NSP_KEY_C,
	NSP_KEY_HOME,
	NSP_KEY_B,
	NSP_KEY_MENU,
	NSP_KEY_A,
	NSP_KEY_ESC,
	NSP_KEY_BAR,
	NSP_KEY_TAB,
	NSP_KEY_EQU,
	NSP_KEY_UP,
	NSP_KEY_UPRIGHT,
	NSP_KEY_RIGHT,
	NSP_KEY_RIGHTDOWN,
	NSP_KEY_DOWN,
	NSP_KEY_DOWNLEFT,
	NSP_KEY_LEFT,
	NSP_KEY_LEFTUP,
	NSP_KEY_SHIFT,
	NSP_KEY_CTRL,
	NSP_KEY_DOC,
	NSP_KEY_TRIG,
	NSP_KEY_SCRATCHPAD,
	NSP_NUMKEYS
};

/* Joystick axes */
enum {
	NSP_JA_H = 0,
	NSP_JA_V,
	NSP_NUMAXES
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
	NSP_NUMBUTTONS
};

/* Fonts; needs to match the nsp_font_charmaps array in SDL_tinspirevideo.c */
enum {
	NSP_FONT_DEFAULT,
	NSP_FONT_SPACE,
	NSP_FONT_VGA,
	NSP_FONT_FANTASY
};

#endif /* _SDL_config_tinspire_h */