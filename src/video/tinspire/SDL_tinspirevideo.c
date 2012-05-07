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

static Uint16 nsp_palette[256] = {0};

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
	"tinspire", "SDL TI-Nspire video driver",
	NSP_Available, NSP_CreateDevice
};

#define NSP_CLEAN_EXIT()	{ NSP_DPRINT("Aborting\n"); \
				  SDL_Quit(); \
				  exit(EXIT_FAILURE); }

int NSP_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	NSP_DPRINT("Initializing video format\n");

	this->hidden->has_touchpad = is_touchpad ? SDL_TRUE : SDL_FALSE;
	this->hidden->use_mouse = ( SDL_strcmp(SDL_getenv("SDL_USEMOUSE"), "1") == 0 && is_touchpad )
				  ? SDL_TRUE : SDL_FALSE;

	/* Warn the user if using mouse but running on a Clickpad */
	if ( ! this->hidden->has_touchpad
	&& SDL_strcmp(SDL_getenv("SDL_WARN_NOMOUSE"), "1") == 0
	&& show_msgbox_2b(NSP_NAME_FULL, "This program requires a mouse, but your calculator does not have a touchpad. "
					 "Some features might not work. Continue at your own risk.",
			  "Return", "Continue") == 1 )
		NSP_CLEAN_EXIT();

	NSP_DPRINT("has_touchpad: %d, use_mouse: %d\n", this->hidden->has_touchpad, this->hidden->use_mouse);

	if ( is_cx ) {
		vformat->BitsPerPixel = 16;
		vformat->Rmask = NSP_RMASK16;
		vformat->Gmask = NSP_GMASK16;
		vformat->Bmask = NSP_BMASK16;
	} else
		vformat->BitsPerPixel = 8;

	return(0);
}

SDL_Rect **NSP_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
	return (SDL_Rect **) -1;
}

SDL_Surface *NSP_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
	Uint32 rmask = 0, gmask = 0, bmask = 0;

	if ( width != SCREEN_WIDTH || height != SCREEN_HEIGHT )
		NSP_DPRINT("Warning: not using 320x240\n");

	if ( bpp < 16 )
		bpp = 8;
	else
		bpp = 16;

	NSP_DPRINT("Initializing display (%dx%dx%d)\n", width, height, bpp);

	if ( bpp == 16 ) {
		if ( is_cx ) {
			rmask = NSP_RMASK16;
			gmask = NSP_GMASK16;
			bmask = NSP_BMASK16;
		} else {
			NSP_DPRINT("Got 16 bpp on TC model, forcing to 8 bpp\n");
			bpp = 8;
		}
	}

	if ( this->hidden->buffer ) {
		SDL_free( this->hidden->buffer );
	}

	this->hidden->buffer = SDL_malloc((bpp / 8) * width * height);
	if ( ! this->hidden->buffer ) {
		SDL_SetError("Couldn't allocate buffer for requested mode");
		return(NULL);
	}

	memset(this->hidden->buffer, 0, (bpp / 8) * width * height);

	/* Allocate the new pixel format for the screen */
	if ( ! SDL_ReallocFormat(current, bpp, rmask, gmask, bmask, 0) ) {
		SDL_free(this->hidden->buffer);
		this->hidden->buffer = NULL;
		SDL_SetError("Couldn't allocate new pixel format for requested mode");
		return(NULL);
	}

	/* Set up the new mode framebuffer */
	current->flags = flags;
	this->hidden->w = current->w = width;
	this->hidden->h = current->h = height;
	current->pitch = (bpp / 8) * current->w;
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

#define NSP_PIXEL_ADDR(origin, x, y, width) (Uint8 *)(origin + (x) + ((y) * width))
#define NSP_DRAW_LOOP(code)	{ \
					while ( rows--) { \
						int j, k; \
						code \
						src_addr += src_skip; \
						dst_addr += dst_skip; \
					} \
				}

static void NSP_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
	int src_skip = SDL_VideoSurface->pitch;
	int dst_skip = is_cx
		     ? (2 * SDL_VideoSurface->w)
		     : (SDL_VideoSurface->w / 2);
	int i;

	for ( i = 0; i < numrects; ++i ) {
		Uint8 *src_addr, *dst_addr;
		int row_bytes, rows;
		SDL_Rect *rect = &rects[i];
		if ( ! rect )
			continue;

		/* Single nibbles go to hell! */
		if ( rect->x & 1 )
			--rect->x;
		if ( rect->w & 1 )
			++rect->w;

		if ( this->hidden->use_mouse )
			if ( SDL_cursor->area.x + 10 > rect->x
			  && SDL_cursor->area.x < rect->x + rect->w
			  && SDL_cursor->area.y + 16 > rect->y
			  && SDL_cursor->area.y < rect->y + rect->h )
				SDL_DrawCursorNoLock(SDL_VideoSurface);

		row_bytes = SDL_VideoSurface->format->BytesPerPixel * rect->w;
		rows = rect->h;

		/* NSP_DPRINT("Updating: (%d, %d) %dx%d\n", rect->x, rect->y, rect->w, rect->h); */

		src_addr = ( SDL_VideoSurface->format->BitsPerPixel == 16 )
			 ? NSP_PIXEL_ADDR(SDL_VideoSurface->pixels,
					  2 * rect->x, 2 * rect->y,
					  SDL_VideoSurface->w)
			 : NSP_PIXEL_ADDR(SDL_VideoSurface->pixels,
					  rect->x, rect->y,
					  SDL_VideoSurface->w);

		dst_addr = is_cx
			 ? NSP_PIXEL_ADDR(SCREEN_BASE_ADDRESS,
			 		  2 * rect->x, 2 * rect->y,
			 		  SDL_VideoSurface->w)
			 : NSP_PIXEL_ADDR(SCREEN_BASE_ADDRESS,
			 		  rect->x / 2, rect->y / 2,
			 		  SDL_VideoSurface->w);

		if ( is_cx ) {
			if ( SDL_VideoSurface->format->BitsPerPixel == 16 ) {
				NSP_DRAW_LOOP(
					SDL_memcpy(dst_addr, src_addr, row_bytes);
				);
			} else {
				NSP_DRAW_LOOP(
					for ( j = 0, k = 0; j < 2 * row_bytes; j += 2, ++k )
						*(Uint16 *)(dst_addr + j) = nsp_palette[src_addr[k]];
				);
			}
		} else {
			NSP_DRAW_LOOP(
				for ( j = 0, k = 0; j < row_bytes; j += 2, ++k )
					dst_addr[k] = (nsp_palette[src_addr[j]] << 4)
						    | nsp_palette[src_addr[j + 1]];
			);
		}
	}
}

static Uint16 nsp_map_rgb_palette(Uint8 r, Uint8 g, Uint8 b) {
	return(is_cx ? (((r / 8) << 11) | ((g / 4) << 5) | (b / 8))
		     : ((r + (2 * g) + b) / 64));
}

int NSP_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	int i;
	for ( i = firstcolor; i < ncolors; ++i )
		nsp_palette[i] = nsp_map_rgb_palette(colors[i].r, colors[i].g, colors[i].b);
	return(1);
}

/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/
void NSP_VideoQuit(_THIS)
{
	NSP_DPRINT("Closing video\n");
}