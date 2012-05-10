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
	return(SDL_SetColors(surface, SDL_VideoSurface->format->palette->colors,
	       0, SDL_VideoSurface->format->palette->ncolors));
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

	NSP_DPRINT("Creating device (%d)", devindex);

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

#define NSP_ABORT() do { \
	NSP_DPRINT("Aborting"); \
	SDL_Quit(); \
	exit(EXIT_FAILURE); \
} while(0)

static int NSP_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	NSP_DPRINT("Initializing video format");

	this->hidden->has_touchpad = is_touchpad ? SDL_TRUE : SDL_FALSE;
	this->hidden->use_mouse = ( SDL_strcmp(SDL_getenv("SDL_USEMOUSE"), "1") == 0 && is_touchpad )
				  ? SDL_TRUE : SDL_FALSE;

	/* Warn the user if using mouse but running on a Clickpad */
	if ( ! this->hidden->has_touchpad
	&& SDL_strcmp(SDL_getenv("SDL_WARN_NOMOUSE"), "1") == 0
	&& show_msgbox_2b(NSP_NAME_FULL, "This program requires a mouse, but your calculator does not have a touchpad. "
					 "Some features might not work. Continue at your own risk.",
			  "Abort", "Continue") == 1 )
		NSP_ABORT();

	NSP_DPRINT("has_touchpad: %d, use_mouse: %d", this->hidden->has_touchpad, this->hidden->use_mouse);

	if ( is_cx ) {
		vformat->BitsPerPixel = 16;
		vformat->Rmask = NSP_RMASK16;
		vformat->Gmask = NSP_GMASK16;
		vformat->Bmask = NSP_BMASK16;
	} else
		vformat->BitsPerPixel = 8;

	return(0);
}

static SDL_Rect **NSP_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
	/* Everything goes; we do the checking in NSP_SetVideoMode() */
	return (SDL_Rect **) -1;
}

static SDL_Surface *NSP_SetVideoMode(_THIS, SDL_Surface *current,
				     int width, int height, int bpp, Uint32 flags)
{
	Uint32 rmask = 0, gmask = 0, bmask = 0;

	width = ( width > SCREEN_WIDTH ) ? SCREEN_WIDTH : width;
	height = ( height > SCREEN_HEIGHT ) ? SCREEN_HEIGHT : height;
	bpp = ( bpp < 16 ) ? 8 : 16;

	NSP_DPRINT("Initializing display (%dx%dx%d)", width, height, bpp);

	if ( width < SCREEN_WIDTH || height < SCREEN_HEIGHT ) {
		int offset_x = (SCREEN_WIDTH - width) / 2;
		int offset_y = (SCREEN_HEIGHT - height) / 2;
		NSP_DPRINT("offset_x: %d, offset_y: %d", offset_x, offset_y);
		this->hidden->offset = is_cx
				     ? (int)NSP_PIXEL_ADDR(0, offset_x, offset_y,
				     			   2 * SCREEN_WIDTH, 2)
				     : (int)NSP_PIXEL_ADDR(0, offset_x / 2, offset_y,
				     			   SCREEN_WIDTH / 2, 1);
		this->hidden->offset_x = offset_x;
		memset(SCREEN_BASE_ADDRESS, 0, SCREEN_BYTES_SIZE);
	} else {
		this->hidden->offset = 0;
		this->hidden->offset_x = 0;
	}

	NSP_DPRINT("Offset: %d", this->hidden->offset);

	if ( bpp == 16 ) {
		if ( is_cx ) {
			rmask = NSP_RMASK16;
			gmask = NSP_GMASK16;
			bmask = NSP_BMASK16;
		} else {
			NSP_DPRINT("Got 16 bpp on TC, forcing to 8 bpp");
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

	NSP_DPRINT("Done (0x%p)", current);

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

#define NSP_DRAW_LOOP(code) do { \
	while ( rows--) { \
		int j = 0, k = 0; \
		code \
		src_addr += src_skip; \
		dst_addr += dst_skip; \
	} \
} while(0)

static void NSP_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
	int src_skip = SDL_VideoSurface->pitch;
	int dst_skip = is_cx ? (2 * SCREEN_WIDTH) : (SCREEN_WIDTH / 2);
	int i;

	for ( i = 0; i < numrects; ++i ) {
		Uint8 *src_addr, *dst_addr;
		int row_bytes, rows;
		int bpp = SDL_VideoSurface->format->BytesPerPixel;
		SDL_bool odd_left, odd_right;
		SDL_Rect *rect = &rects[i];
		if ( ! rect )
			continue;

		if ( this->hidden->use_mouse )
			if ( SDL_cursor->area.x + 10 > rect->x
			  && SDL_cursor->area.x < rect->x + rect->w
			  && SDL_cursor->area.y + 16 > rect->y
			  && SDL_cursor->area.y < rect->y + rect->h )
				SDL_DrawCursorNoLock(SDL_VideoSurface);

		row_bytes = bpp * rect->w;
		rows = rect->h;
		odd_left = (this->hidden->offset_x + rect->x) & 1;
		odd_right = (this->hidden->offset_x + rect->x + rect->w) & 1;
		if ( is_classic && odd_right )
			--row_bytes;

		/* NSP_DPRINT("Updating: (%d, %d) %dx%d", rect->x, rect->y, rect->w, rect->h); */

		src_addr = NSP_SURF_PIXEL(SDL_VideoSurface, rect->x, rect->y);

		dst_addr = is_cx
			 ? NSP_PIXEL_ADDR(SCREEN_BASE_ADDRESS,
			 		  rect->x, rect->y,
			 		  2 * SCREEN_WIDTH, 2)
			 : NSP_PIXEL_ADDR(SCREEN_BASE_ADDRESS,
			 		  rect->x / 2, rect->y,
			 		  SCREEN_WIDTH / 2, 1);
		dst_addr += this->hidden->offset;

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
				if ( odd_left ) {
					*dst_addr = (*dst_addr & 0xf0) | nsp_palette[*src_addr];
					++j;
					++k;
				}
				for ( ; j < row_bytes; j += 2, ++k )
					dst_addr[k] = (nsp_palette[src_addr[j]] << 4)
						    | nsp_palette[src_addr[j + 1]];
				if ( odd_right )
					dst_addr[k] = (nsp_palette[src_addr[j]] << 4) | (dst_addr[k] & 0x0f);
			);
		}
	}
}

static Uint16 nsp_map_rgb_palette(Uint8 r, Uint8 g, Uint8 b) {
	return(is_cx ? (((r / 8) << 11) | ((g / 4) << 5) | (b / 8))
		     : ((r + (2 * g) + b) / 64));
}

static int NSP_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	int i;
	for ( i = firstcolor; i < ncolors; ++i )
		nsp_palette[i] = nsp_map_rgb_palette(colors[i].r, colors[i].g, colors[i].b);
	return(1);
}

/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/
static void NSP_VideoQuit(_THIS)
{
	NSP_DPRINT("Closing video");
}