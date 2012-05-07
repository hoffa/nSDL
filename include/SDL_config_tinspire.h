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

#define NSP_RMASK16	0xF800
#define NSP_GMASK16	0x07E0
#define NSP_BMASK16	0x001F

#if 1
#define NSP_DEBUG	1
#define DEBUG_BUILD	1
// #define DEBUG_PALETTE	1
#define DEBUG_VIDEO	1
#define DEBUG_ASM	1
// #define DEBUG_GRAB	1
#define DEBUG_TIMERS	1
#define DEBUG_QSORT	1
#define DEBUG_THREADS	1
#define DEBUG_ERROR	1
#define DEBUG_IMGLIB	1
#endif

#define NSP_NAME	"nSDL"
#define NSP_VERSION	"0.3.0"
#define NSP_NAME_FULL	(NSP_NAME " " NSP_VERSION)

#if NSP_DEBUG
#define NSP_DPRINT(fmt, args...) \
		fprintf(stderr, "[NSP] %s():%d: " fmt, \
			__FUNCTION__, __LINE__, ## args)
#else
#define NSP_DPRINT(fmt, args...) (void)0
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

/* Allow disabling of core subsystems */
#define SDL_AUDIO_DISABLED	1
#define SDL_CDROM_DISABLED	1
#define SDL_JOYSTICK_DISABLED	1
#define SDL_LOADSO_DISABLED	1
#define SDL_THREADS_DISABLED	1

/* Enable various timer systems */
#define SDL_TIMER_TINSPIRE	1

/* Enable various video drivers */
#define SDL_VIDEO_DRIVER_TINSPIRE	1

/* Enable assembly routines */
#define SDL_ASSEMBLY_ROUTINES	1

/* Font flags */
#define NSP_FONTCFG_NOTHING	0
#define NSP_FONTCFG_TEXTWRAP	(1 << 0)
#define NSP_FONTCFG_AUTOSIZE	(1 << 1)
#define NSP_FONTCFG_FORMAT	(1 << 2)
#define NSP_FONTCFG_DEFAULT	(NSP_FONTCFG_TEXTWRAP | NSP_FONTCFG_AUTOSIZE | NSP_FONTCFG_FORMAT)

/* Fonts; needs to match nsp_font_charmaps in SDL_tinspirevideo.c */
enum {
	NSP_FONT_THIN = 0,
	NSP_FONT_SPACE,
	NSP_FONT_VGA,
	NSP_FONT_FANTASY,
	NSP_FONT_TINYTYPE
};

#endif /* _SDL_config_tinspire_h */