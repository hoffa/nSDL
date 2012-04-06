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
#include "SDL_tinspirefonts.h"

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

static unsigned char *nsp_font_charmaps[] = {
	nsp_font_thin,
	nsp_font_space,
	nsp_font_vga,
	nsp_font_fantasy,
	nsp_font_tinytype
};

#if NSP_CX_8BIT
static Uint16 nsp_palette[256] = {NSP_MAP(255, 255, 255)};
#elif NSP_TC
static Uint8 nsp_palette[256] = {NSP_MAP(255, 255, 255)};
#endif

#define NSP_TAB_WIDTH_PXL	(NSP_TAB_WIDTH * NSP_FONT_WIDTH)

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

SDL_nFont *SDL_nLoadFont(int font_index, Uint32 color, Uint32 flags) {
	unsigned char *charmap = nsp_font_charmaps[font_index];
	int i, j, k;
	SDL_nFont *font = SDL_malloc(sizeof *font);
	if ( font == NULL ) {
		SDL_OutOfMemory();
		return(NULL);
	}
	for ( i = 0; i < NSP_FONT_NUMCHARS; ++i ) {
		int offset = i << 3;
		int max_width = 0;
		SDL_Surface *char_surf = SDL_CreateRGBSurface(SDL_SWSURFACE, NSP_FONT_WIDTH,
			NSP_FONT_HEIGHT, NSP_BPP, NSP_RMASK, NSP_GMASK, NSP_BMASK, 0);
		if ( char_surf == NULL )
			return(NULL);
		font->char_width[i] = NSP_FONT_WIDTH;
#if !NSP_CX_16BIT
		if ( ! SDL_nCreatePalette(char_surf) ) {
			SDL_FreeSurface(char_surf);
			return(NULL);
		}
#endif
		if ( SDL_SetColorKey(char_surf, SDL_SRCCOLORKEY | SDL_RLEACCEL, 0) == -1 ) {
			SDL_FreeSurface(char_surf);
			return(NULL);
		}
		SDL_LockSurface(char_surf);
		for ( j = 0; j < 8; ++j )
			for ( k = 0; k < 8; ++k ) {
				if ( charmap[offset + j] & (1 << (7 - k)) ) { /* "Pixel" set */
					if ( k > max_width ) {
						font->char_width[i] = k + 1;
						max_width = k;
					}
#if NSP_CX_16BIT
					*(Uint16 *)(char_surf->pixels + (k << 1) + (j << 4)) = (Uint16)color;
#else
					*(Uint8 *)(char_surf->pixels + k + (j << 3)) = (Uint8)color;
#endif
				}
			}
		SDL_UnlockSurface(char_surf);
		font->chars[i] = char_surf;
		font->hspacing = NSP_HSPACING_DEFAULT;
		font->vspacing = NSP_VSPACING_DEFAULT;
		font->flags = flags;
	}
	return(font);
}

void SDL_nSetFontSpacing(SDL_nFont *font, int hspacing, int vspacing) {
	font->hspacing = hspacing;
	font->vspacing = vspacing;
}

void SDL_nFreeFont(SDL_nFont *font) {
	int i;
	for ( i = 0; i < NSP_FONT_NUMCHARS; ++i )
		SDL_FreeSurface(font->chars[i]);
	SDL_free(font);
}

int SDL_nDrawChar(SDL_Surface *surface, SDL_nFont *font, SDL_Rect *pos, int c) {
	SDL_Rect rect = {0, 0, 0, 0};
	rect.w = font->char_width[c];
	rect.h = NSP_FONT_HEIGHT;
	return(SDL_BlitSurface(font->chars[c], &rect, surface, pos));
}

#define NSP_BUF_SIZE	512

int SDL_nDrawString(SDL_Surface *surface, SDL_nFont *font, int x, int y, const char *format, ...) {
	int length;
	int i;
	SDL_Rect pos;
	char buffer[NSP_BUF_SIZE];
	va_list args;

	va_start(args, format);
	vsprintf(buffer, format, args); /* Warning: possibility of overflow */
	va_end(args);
	pos.x = x;
	pos.y = y;
	length = (int)strlen(buffer);
	for ( i = 0; i < length; ++i ) {
		int char_w = NSP_FONT_WIDTH;
		int c = buffer[i];
		if ( font->flags & NSP_FONT_AUTOSIZE )
			char_w = font->char_width[c];
		switch ( c ) {
			case '\n':
				pos.x = x;
				pos.y += NSP_FONT_HEIGHT + font->vspacing;
				break;
			case '\t':
				pos.x += NSP_TAB_WIDTH_PXL - (pos.x % NSP_TAB_WIDTH_PXL);
				break;
			default:
				if ( SDL_nDrawChar(surface, font, &pos, c) == -1 )
					return(-1);
				if ( i < length - 1 ) {
					if ( (font->flags & NSP_FONT_TEXTWRAP)
					&& pos.x + char_w + font->hspacing + font->char_width[buffer[i + 1]] >= surface->w ) {
						pos.x = 0;
						if ( buffer[i + 1] != '\n' )
							pos.y += NSP_FONT_HEIGHT;
						pos.y += font->vspacing;
					} else
						pos.x += char_w + font->hspacing;
				}
				break;
		}
	}
	return(0);
}

int SDL_nGetLineWidth(SDL_nFont *font, const char *s) {
	int width = 0;
	int i;
	if ( *s == '\n' )
		return 0;
	for ( i = 0; s[i] && s[i] != '\n'; ++i ) {
		int char_w = NSP_FONT_WIDTH;
		if ( s[i] == '\t' )
			char_w = NSP_TAB_WIDTH_PXL;
		else if ( font->flags & NSP_FONT_AUTOSIZE )
			char_w = font->char_width[s[i]];
		width += char_w + font->hspacing;
	}
	if ( ! (font->flags & NSP_FONT_AUTOSIZE) )
		width -= NSP_FONT_WIDTH - font->char_width[s[i - 1]];
	return width - font->hspacing;
}

int SDL_nGetStringWidth(SDL_nFont *font, const char *s) {
	int length = (int)strlen(s);
	int max_width = SDL_nGetLineWidth(font, s);
	int i;
	for ( i = 0; i < length; ++i )
		if ( i > 0 && s[i - 1] == '\n' ) {
			int width = SDL_nGetLineWidth(font, &s[i]);
			if ( width > max_width )
				max_width = width;
		}
	return max_width;
}

int SDL_nGetStringHeight(SDL_nFont *font, const char *s) {
	int length = (int)strlen(s);
	int height = NSP_FONT_HEIGHT;
	int i;
	for ( i = 0; i < length; ++i )
		if ( s[i] == '\n' )
			height += NSP_FONT_HEIGHT + font->vspacing;
	return height;
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

	NSP_NL_RELOCDATA(nsp_font_charmaps, NSP_ARRAY_SIZE(nsp_font_charmaps));
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
