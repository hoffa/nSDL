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

#include "SDL.h"
#include "SDL_config.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"
#include "../SDL_cursor_c.h"

#include "SDL_tinspirevideo.h"
#include "SDL_tinspireevents_c.h"

#if NSP_BPP_SW8_HW16
#define NSP_MAP_RGB(r, g, b)	(Uint16)(((r / 8) << 11) | ((g / 4) << 5) | (b / 8))
static Uint16 nsp_palette[256] = {0x0000};
#elif NSP_BPP_SW8_HW4
#define NSP_MAP_RGB(r, g, b)	(Uint8)((r + (2 * g) + b) / 64) /* ((r + g + b) / 48) */
static Uint8 nsp_palette[256] = {0x00};
#endif

/* Initialization/Query functions */
static int NSP_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **NSP_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
static SDL_Surface *NSP_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static int NSP_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors);
static void NSP_VideoQuit(_THIS);

/* Hardware surface functions */
static int NSP_AllocHWSurface(_THIS, SDL_Surface *surface);
static int NSP_LockHWSurface(_THIS, SDL_Surface *surface);
static void NSP_UnlockHWSurface(_THIS, SDL_Surface *surface);
static void NSP_FreeHWSurface(_THIS, SDL_Surface *surface);

/* WM functions */
static void NSP_MoveWMCursor(_THIS, int x, int y);

/* etc. */
static void NSP_UpdateRects(_THIS, int numrects, SDL_Rect *rects);

#if NSP_BPP_SW8_HW8
#if NSP_CX
#define NSP_MAP_PALETTE(r, g, b)	(((r / 8) << 10) | ((g / 8) << 5) | (b / 8))
#else
#define NSP_MAP_PALETTE(r, g, b)	((15 - ((r + (2 * g) + b) / 64)) << 1)
#endif
static Uint8 *nsp_lcd_buffer;
static Uint16 nsp_hw_palette[256] = {0x0000};
static Uint16 nsp_orig_hw_palette[16];
static unsigned *nsp_orig_base;
static volatile unsigned *nsp_lcd_ctrl;
#endif

int SDL_nCreatePalette(SDL_Surface *surface)
{
	SDL_Color colors[256];
	int i;
	for ( i = 0; i < 256; ++i )
		colors[i].r = colors[i].g = colors[i].b = i;
	if ( ! SDL_SetColors(surface, colors, 0, 256) ) {
		SDL_SetError("[NSP] Couldn't create palette");
		return(0);
	}
	return(1);
}

/* NSP driver bootstrap functions */

static int NSP_Available(void)
{
	return(1);
}

static void NSP_DeleteDevice(SDL_VideoDevice *device)
{
	SDL_free(device->hidden);
	SDL_free(device);
}

static SDL_VideoDevice *NSP_CreateDevice(int devindex)
{
	SDL_VideoDevice *device;

	NSP_DPRINT("Creating device (%d)\n", devindex);

	/* Initialize all variables that we clean on shutdown */
	device = (SDL_VideoDevice *)SDL_malloc(sizeof(SDL_VideoDevice));
	if ( device ) {
		SDL_memset(device, 0, (sizeof *device));
		device->hidden = (struct SDL_PrivateVideoData *)
				SDL_malloc((sizeof *device->hidden));
	}
	if ( (device == NULL) || (device->hidden == NULL) ) {
		SDL_OutOfMemory();
		if ( device ) {
			SDL_free(device);
		}
		return(0);
	}
	SDL_memset(device->hidden, 0, (sizeof *device->hidden));

	/* Set the function pointers */
	device->VideoInit = NSP_VideoInit;
	device->ListModes = NSP_ListModes;
	device->SetVideoMode = NSP_SetVideoMode;
	device->CreateYUVOverlay = NULL;
	device->SetColors = NSP_SetColors;
	device->UpdateRects = NSP_UpdateRects;
	device->VideoQuit = NSP_VideoQuit;
	device->AllocHWSurface = NSP_AllocHWSurface;
	device->CheckHWBlit = NULL;
	device->FillHWRect = NULL;
	device->SetHWColorKey = NULL;
	device->SetHWAlpha = NULL;
	device->LockHWSurface = NSP_LockHWSurface;
	device->UnlockHWSurface = NSP_UnlockHWSurface;
	device->FlipHWSurface = NULL;
	device->FreeHWSurface = NSP_FreeHWSurface;
	device->SetCaption = NULL;
	device->SetIcon = NULL;
	device->IconifyWindow = NULL;
	device->GrabInput = NULL;
	device->InitOSKeymap = NSP_InitOSKeymap;
	device->PumpEvents = NSP_PumpEvents;
	device->GetWMInfo = NULL;
	device->CreateWMCursor = NULL;
	device->UpdateMouse = NULL;
	device->FreeWMCursor = NULL;
	device->ShowWMCursor = NULL;
	device->WarpWMCursor = NULL;
	device->MoveWMCursor = NSP_MoveWMCursor;
	device->CheckMouseMode = NULL;

	device->free = NSP_DeleteDevice;

	return device;
}

VideoBootStrap NSP_bootstrap = {
	"NSPVID", "SDL TI-Nspire video driver",
	NSP_Available, NSP_CreateDevice
};

#define NSP_CLEAN_EXIT()	{ NSP_DPRINT("Aborting\n"); \
				  SDL_Quit(); \
				  exit(EXIT_FAILURE); }
#define NSP_MSG_INCOMP_CALC(color_or_grayscale)	"Pixel format not supported.\nThis program has been " \
						"compiled for TI-Nspire calculators with a " color_or_grayscale " display."

int NSP_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	NSP_DPRINT("Initializing video format\n");

/* Check pixel format compatibility */
#if NSP_CX
	if ( is_classic ) {
		show_msgbox(NSP_NAME_FULL, NSP_MSG_INCOMP_CALC("color"));
		NSP_CLEAN_EXIT();
	}
#else
	if ( is_cx ) {
		show_msgbox(NSP_NAME_FULL, NSP_MSG_INCOMP_CALC("grayscale"));
		NSP_CLEAN_EXIT();
	}
#endif

/* Warn the user if using mouse but running on a Clickpad */
if ( ! is_touchpad
&& SDL_strcmp(SDL_getenv("SDL_WARN_NOMOUSE"), "1") == 0
&& show_msgbox_2b(NSP_NAME_FULL, "This program requires a mouse, but your calculator does not have a touchpad. "
				 "Some features might not work. Continue at your own risk.",
		  "Return", "Continue") == 1 )
	NSP_CLEAN_EXIT();

this->hidden->use_mouse = ( SDL_strcmp(SDL_getenv("SDL_USEMOUSE"), "1") == 0 && is_touchpad )
			  ? SDL_TRUE : SDL_FALSE;
this->hidden->is_clickpad = is_touchpad ? SDL_FALSE : SDL_TRUE;

NSP_DPRINT("use_mouse: %d, is_clickpad: %d\n", this->hidden->use_mouse, this->hidden->is_clickpad);

#if NSP_BPP_SW8_HW8
	NSP_DPRINT("Switching to hardware 8 bpp\n");

	nsp_lcd_ctrl = IO_LCD_CONTROL;
	nsp_lcd_buffer = SDL_malloc(NSP_LCDBUF_SIZE);
	if ( nsp_lcd_buffer == NULL ) {
		SDL_OutOfMemory();
		NSP_CLEAN_EXIT();
	}
	SDL_memset(nsp_lcd_buffer, 0, NSP_LCDBUF_SIZE);

	nsp_orig_base = *(volatile unsigned *)NSP_BASE_ADDR;
	*nsp_lcd_ctrl &= ~0b000001110;
	*nsp_lcd_ctrl |=  0b000000110;
	*(volatile unsigned *)NSP_BASE_ADDR = (unsigned)nsp_lcd_buffer;

	SDL_memcpy(nsp_orig_hw_palette, (unsigned *)NSP_PALETTE_ADDR, 32);
#endif

	vformat->BitsPerPixel = NSP_BPP;
	vformat->BytesPerPixel = NSP_BYTESPP;
	vformat->Rmask = NSP_RMASK;
	vformat->Gmask = NSP_GMASK;
	vformat->Bmask = NSP_BMASK;

	return(0);
}

SDL_Rect **NSP_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
   	return (SDL_Rect **) -1;
}

SDL_Surface *NSP_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
	if ( width != SCREEN_WIDTH || height != SCREEN_HEIGHT )
		NSP_DPRINT("Warning: not using 320x240, weird stuff might happen\n");

	/* Sorry bro, we don't actually care about bpp, NSP_BPP is how we roll! */
	if ( bpp != NSP_BPP )
		NSP_DPRINT("Warning: got %d bpp, forcing to %d bpp\n", bpp, NSP_BPP);

	NSP_DPRINT("Initializing display (%dx%dx%d)\n", width, height, NSP_BPP);

	if ( this->hidden->buffer ) {
		SDL_free( this->hidden->buffer );
	}

	this->hidden->buffer = SDL_malloc(NSP_BYTESPP * width * height);
	if ( ! this->hidden->buffer ) {
		SDL_SetError("Couldn't allocate buffer for requested mode");
		return(NULL);
	}

	memset(this->hidden->buffer, 0, NSP_BYTESPP * width * height);

	/* Allocate the new pixel format for the screen */
	if ( ! SDL_ReallocFormat(current, NSP_BPP, NSP_RMASK, NSP_GMASK, NSP_BMASK, 0) ) {
		SDL_free(this->hidden->buffer);
		this->hidden->buffer = NULL;
		SDL_SetError("Couldn't allocate new pixel format for requested mode");
		return(NULL);
	}

	/* Set up the new mode framebuffer */
	current->flags = flags;
	this->hidden->w = current->w = width;
	this->hidden->h = current->h = height;
	current->pitch = NSP_BYTESPP * current->w;
	current->pixels = this->hidden->buffer;

	NSP_DPRINT("Done (0x%p)\n", current);

	/* We're done */
	return(current);
}

/* We don't actually allow hardware surfaces other than the main one */
static int NSP_AllocHWSurface(_THIS, SDL_Surface *surface)
{
	return(-1);
}
static void NSP_FreeHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}

/* We need to wait for vertical retrace on page flipped displays */
static int NSP_LockHWSurface(_THIS, SDL_Surface *surface)
{
	return(0);
}

static void NSP_UnlockHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}

static void NSP_MoveWMCursor(_THIS, int x, int y) {
	SDL_cursor->area.x = x;
	SDL_cursor->area.y = y;
}

static void NSP_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
	const int src_skip = SDL_VideoSurface->w;
	int i;

#if NSP_BPP_SW8_HW4
	const int dst_skip = SDL_VideoSurface->w / 2;
#else
	const int dst_skip = SDL_VideoSurface->w;
#endif

#if NSP_BPP_SW16
	Uint16 *src_addr;
#else
	Uint8 *src_addr;
#endif

#if NSP_BPP_SW16_HW16 || NSP_BPP_SW8_HW16
	Uint16 *dst_addr;
#else
	Uint8 *dst_addr;
#endif

	for ( i = 0; i < numrects; ++i ) {
		int row_bytes, rows;
		SDL_Rect *rect = &rects[i];
		if ( ! rect )
			continue;

		if ( this->hidden->use_mouse )
			if ( SDL_cursor->area.x + 10 > rect->x
			  && SDL_cursor->area.x < rect->x + rect->w
			  && SDL_cursor->area.y + 16 > rect->y
			  && SDL_cursor->area.y < rect->y + rect->h )
				SDL_DrawCursorNoLock(SDL_VideoSurface);

#if NSP_BPP_SW8_HW4
		/* Single nibbles go to hell! */
		if ( rect->x & 1 )
			--rect->x;
		if ( rect->w & 1 )
			++rect->w;
#endif

		row_bytes = NSP_BYTESPP * rect->w;
		rows = rect->h;

		/* NSP_DPRINT("Updating: (%d, %d) %dx%d\n", rect->x, rect->y, rect->w, rect->h); */

#if NSP_BPP_SW16
		src_addr = (Uint16 *)(SDL_VideoSurface->pixels + rect->x + (rect->y * SDL_VideoSurface->w));
#else
		src_addr = (Uint8 *)(SDL_VideoSurface->pixels + rect->x + (rect->y * SDL_VideoSurface->w));
#endif

#if NSP_BPP_SW16_HW16 || NSP_BPP_SW8_HW16
		dst_addr = (Uint16 *)(SCREEN_BASE_ADDRESS + rect->x + (rect->y * SDL_VideoSurface->w));
#else
		dst_addr = (Uint8 *)(SCREEN_BASE_ADDRESS + rect->x + (rect->y * SDL_VideoSurface->w));
#endif

		while ( rows-- ) {
#if NSP_BPP_SW16_HW16 || NSP_BPP_SW8_HW8
			memcpy(dst_addr, src_addr, row_bytes);
#elif NSP_BPP_SW8_HW16
			int j;
			for ( j = 0; j < row_bytes; ++j )
				dst_addr[j] = nsp_palette[src_addr[j]];
#elif NSP_BPP_SW8_HW4
			int j, k;
			for ( j = 0, k = 0; j < row_bytes; j += 2, ++k )
				dst_addr[k] = (nsp_palette[src_addr[j]] << 4) | nsp_palette[src_addr[j + 1]];
#endif
			src_addr += src_skip;
			dst_addr += dst_skip;
		}
	}
}

int NSP_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	int i;
	for ( i = firstcolor; i < ncolors; ++i )
#if NSP_BPP_SW8_HW16 || NSP_BPP_SW8_HW4
		nsp_palette[i] = NSP_MAP_RGB(colors[i].r, colors[i].g, colors[i].b);
#elif NSP_BPP_SW8_HW8
		nsp_hw_palette[i] = (Uint16)NSP_MAP_PALETTE(colors[i].r, colors[i].g, colors[i].b);
	memcpy((unsigned *)NSP_PALETTE_ADDR, nsp_hw_palette, 512);
#else
		return(1);
#endif
	return(1);
}

/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/
void NSP_VideoQuit(_THIS)
{
	NSP_DPRINT("Closing video\n");

#if NSP_BPP_SW8_HW8
	NSP_DPRINT("Quitting hardware 8 bpp\n");
#if NSP_CX
	*nsp_lcd_ctrl |= 0b100000000;
	lcd_incolor();
#else
	lcd_ingray();
#endif
	*(volatile unsigned *)NSP_BASE_ADDR = (unsigned)nsp_orig_base;
	SDL_memcpy((unsigned *)NSP_PALETTE_ADDR, nsp_orig_hw_palette, 32);
	SDL_free(nsp_lcd_buffer);
#endif
}