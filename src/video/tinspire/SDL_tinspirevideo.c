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
#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include "SDL_tinspirevideo.h"
#include "SDL_tinspireevents_c.h"

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

int nsp_create_palette(SDL_Surface *surface) {
	SDL_Color colors[256];
	int i;
	for ( i = 0; i < 256; ++i )
		colors[i].r = colors[i].g = colors[i].b = i;
	return SDL_SetColors(surface, colors, 0, 256);
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

	NSP_DPRINT("Creating device\n");

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
	device->GetWMInfo = NULL;
	device->InitOSKeymap = NSP_InitOSKeymap;
	device->PumpEvents = NSP_PumpEvents;

	device->free = NSP_DeleteDevice;

	return device;
}

VideoBootStrap NSP_bootstrap = {
	"NSPVID", "SDL TI-Nspire video driver",
	NSP_Available, NSP_CreateDevice
};

/* FIXME: CreateRGBSurface might be the source of problems */
int NSP_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	NSP_DPRINT("Initializing video format\n");

#if NSP_COLOR_LCD
	if ( is_classic ) {
		show_msgbox("SDL", "Pixel format not supported.\nThis program has been " \
				"compiled for TI-Nspire calculators with a color display.");
		SDL_SetError("[NSP] Pixel format not supported");
		return(-1);
	}
#else
	if ( is_cx ) {
		show_msgbox("SDL", "Pixel format not supported.\nThis program has been " \
				"compiled for TI-Nspire calculators with a grayscale display.");
		SDL_SetError("[NSP] Pixel format not supported");
		return(-1);
	}
#endif
	/* Non-CX is 4 BPP; if not CX we pretend it's 8 BPP and do the
	conversion at the very end */
	vformat->BitsPerPixel = NSP_BPP;
	vformat->BytesPerPixel = vformat->BitsPerPixel / 8;
	vformat->Rmask = NSP_RMASK;
	vformat->Gmask = NSP_GMASK;
	vformat->Bmask = NSP_BMASK;

	NSP_DPRINT("BitsPerPixel: %d, BytesPerPixel: %d, Rmask: 0x%x, Gmask: 0x%x, Bmask: 0x%x, Amask: 0x%x\n",
		vformat->BitsPerPixel, vformat->BytesPerPixel, vformat->Rmask, vformat->Gmask, vformat->Bmask, vformat->Amask);

	return(0);
}

SDL_Rect **NSP_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
   	return (SDL_Rect **) -1;
}

/* FIXME: Memory leak with non-CX */
SDL_Surface *NSP_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
	if ( bpp <= 8 )
		bpp = 8;
	else
		bpp = 16;

	NSP_DPRINT("Initializing display (%dx%dx%d)\n", width, height, bpp);

	if ( this->hidden->buffer ) {
		SDL_free( this->hidden->buffer );
	}

	this->hidden->buffer = SDL_malloc(NSP_DBL_IF_CX(width * height));
	if ( ! this->hidden->buffer ) {
		SDL_SetError("Couldn't allocate buffer for requested mode");
		return(NULL);
	}

	memset(this->hidden->buffer, 0, NSP_DBL_IF_CX(width * height));

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
	current->pitch = NSP_DBL_IF_CX(current->w);
	current->pixels = this->hidden->buffer;
#if !NSP_COLOR_LCD
	/* This isn't actually needed, but I'll keep it here to keep SDL happy */
	if ( ! nsp_create_palette(current) ) {
		SDL_SetError("[NSP] Couldn't create palette");
		return(NULL);
	}
#endif
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

/* FIXME: odd positions in non-CX etc. */
static void NSP_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
	Uint8 *src_addr, *dst_addr;
	int rect_x, rect_y, rect_w, rect_h;
	int i;
#if !NSP_COLOR_LCD
	int j;
	int dst_skip = SDL_VideoSurface->w >> 1;
#endif
	for ( i = 0; i < numrects; ++i ) {
		SDL_Rect *rect = &rects[i];
		if ( ! rect )
			continue;
#if !NSP_COLOR_LCD
		/* Single nibbles go to hell! */
		if ( rect->x & 1 )
			--rect->x;
		if ( rect->w & 1 )
			++rect->w;
#endif
		rect_x = NSP_DBL_IF_CX(rect->x);
		rect_y = NSP_DBL_IF_CX(rect->y);
		rect_w = NSP_DBL_IF_CX(rect->w);
		rect_h = rect->h;
		NSP_DPRINT("Updating: (%d, %d) %dx%d\n", rect->x, rect->y, rect->w, rect->h);
		src_addr = (Uint8 *)(SDL_VideoSurface->pixels + rect_x + (rect_y * SDL_VideoSurface->w));
#if NSP_COLOR_LCD
		dst_addr = (Uint8 *)(SCREEN_BASE_ADDRESS + rect_x + (rect_y * SDL_VideoSurface->w));
#else
		dst_addr = (Uint8 *)(SCREEN_BASE_ADDRESS + (rect_x >> 1) + ((rect_y >> 1) * SDL_VideoSurface->w));
#endif
		while ( rect_h-- ) {
#if NSP_COLOR_LCD
			memcpy(dst_addr, src_addr, rect_w);
			src_addr += SDL_VideoSurface->pitch;
			dst_addr += SDL_VideoSurface->pitch;
#else
			for ( j = 0; j < rect_w; j += 2 )
				dst_addr[j >> 1] = (src_addr[j] << 4) | src_addr[j + 1];
			src_addr += SDL_VideoSurface->w;
			dst_addr += dst_skip;
#endif
		}
	}
}

int NSP_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	/* do nothing of note. */
	return(1);
}

/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/
/* FIXME: If I use the dummy version of this function (i.e. the one that frees
   this->screen->pixels) the calculator reboots if SDL_Init fails. I'm not sure
   if anything should even be here; DC and NDS don't do anything at the end as
   far as this->screen->pixels is concerned.
*/
void NSP_VideoQuit(_THIS)
{
	NSP_DPRINT("Closing video\n");
}
