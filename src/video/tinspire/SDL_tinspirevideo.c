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

#if NSP_CX_8BIT
#define NSP_MAP(r, g, b)	(Uint16)(((r / 8) << 11) | ((g / 4) << 5) | (b / 8))
#elif NSP_TC
#define NSP_MAP(r, g, b)	((r + g + b) / 48)
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

/* etc. */
static void NSP_UpdateRects(_THIS, int numrects, SDL_Rect *rects);

#if NSP_CX_8BIT
static Uint16 nsp_palette[256] = {NSP_MAP(255, 255, 255)};
#elif NSP_TC
static Uint8 nsp_palette[256] = {NSP_MAP(255, 255, 255)};
#endif

int SDL_nCreatePalette(SDL_Surface *surface) {
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
	device->MoveWMCursor = NULL;
	device->CheckMouseMode = NULL;

	device->free = NSP_DeleteDevice;

	return device;
}

VideoBootStrap NSP_bootstrap = {
	"NSPVID", "SDL TI-Nspire video driver",
	NSP_Available, NSP_CreateDevice
};

#define NSP_MSG_INCOMP_CALC(color_or_grayscale)	"Pixel format not supported.\nThis program has been " \
						"compiled for TI-Nspire calculators with a " color_or_grayscale " display."

int NSP_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	NSP_DPRINT("Initializing video format\n");

	vformat->BitsPerPixel = NSP_BPP;
	vformat->BytesPerPixel = NSP_BYTESPP;
	vformat->Rmask = NSP_RMASK;
	vformat->Gmask = NSP_GMASK;
	vformat->Bmask = NSP_BMASK;

#if NSP_CX
	if ( is_classic ) {
		show_msgbox(NSP_NAME_FULL, NSP_MSG_INCOMP_CALC("color"));
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
#else
	if ( is_cx ) {
		show_msgbox(NSP_NAME_FULL, NSP_MSG_INCOMP_CALC("grayscale"));
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
#endif

	return(0);
}

SDL_Rect **NSP_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
   	return (SDL_Rect **) -1;
}

SDL_Surface *NSP_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
	NSP_DPRINT("Initializing display (%dx%dx%d)\n", width, height, bpp);

	if ( bpp <= 8 )
		bpp = 8;
	else
		bpp = 16;

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
	if ( ! SDL_ReallocFormat(current, bpp, NSP_RMASK, NSP_GMASK, NSP_BMASK, 0) ) {
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

static void NSP_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
	Uint8 *src_addr;
	int i;
#if NSP_CX_16BIT
	const int src_skip = SDL_VideoSurface->pitch;
#else
	const int src_skip = SDL_VideoSurface->w;
#if NSP_CX_8BIT
	SDL_Palette *palette = SDL_VideoSurface->format->palette;
#endif
#endif
#if NSP_CX
	const int dst_skip = SDL_VideoSurface->w;
	Uint16 *dst_addr;
#else
	const int dst_skip = SDL_VideoSurface->w / 2;
	Uint8 *dst_addr;
#endif

	for ( i = 0; i < numrects; ++i ) {
		int rect_w, rect_h;
		SDL_Rect *rect = &rects[i];
		if ( ! rect )
			continue;

		/* Single nibbles go to hell! (not required for CX, but keeps
		   everything consistent) */
		if ( rect->x & 1 )
			--rect->x;
		if ( rect->w & 1 )
			++rect->w;

		rect_w = NSP_BYTESPP * rect->w;
		rect_h = rect->h;

		/* NSP_DPRINT("Updating: (%d, %d) %dx%d\n", rect->x, rect->y, rect->w, rect->h); */
		src_addr = (Uint8 *)(SDL_VideoSurface->pixels + (NSP_BYTESPP * rect->x)
			 + ((NSP_BYTESPP * rect->y) * SDL_VideoSurface->w));
#if NSP_CX
		dst_addr = (Uint16 *)(SCREEN_BASE_ADDRESS + rect->x + (rect->y * SDL_VideoSurface->w));
#else
		dst_addr = (Uint8 *)(SCREEN_BASE_ADDRESS + rect->x + (rect->y * SDL_VideoSurface->w));
#endif
		while ( rect_h-- ) {
#if NSP_CX_8BIT || NSP_TC
			int j, k;
#endif
#if NSP_CX_16BIT
			memcpy(dst_addr, src_addr, rect_w);
#elif NSP_CX_8BIT
			for ( j = 0; j < rect->w; ++j )
				dst_addr[j] = nsp_palette[src_addr[j]];
#else
			for ( j = 0, k = 0; j < rect_w; j += 2, ++k )
				dst_addr[k] = (nsp_palette[src_addr[j]] << 4) | nsp_palette[src_addr[j + 1]];
#endif
			src_addr += src_skip;
			dst_addr += dst_skip;
		}
	}
}

int NSP_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
#if NSP_CX_8BIT || NSP_TC
	int i;
	for ( i = firstcolor; i < ncolors; ++i )
		nsp_palette[i] = NSP_MAP(colors[i].r, colors[i].g, colors[i].b);
#endif
	return(1);
}

/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/
void NSP_VideoQuit(_THIS)
{
	NSP_DPRINT("Closing video\n");
}
