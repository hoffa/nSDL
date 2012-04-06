#include "SDL_config.h"
#include "../SDL_sysvideo.h"
#include "SDL_tinspirefonts.h"

#define NSP_TAB_WIDTH_PXL	(NSP_TAB_WIDTH * NSP_FONT_WIDTH)

static unsigned char *nsp_font_charmaps[] = {
	nsp_font_thin,
	nsp_font_space,
	nsp_font_vga,
	nsp_font_fantasy,
	nsp_font_tinytype
};

static SDL_bool charmap_relocated = SDL_FALSE;

SDL_nFont *SDL_nLoadFont(int font_index, Uint32 color, Uint32 flags) {
	unsigned char *charmap;
	int i, j, k;
	SDL_nFont *font = SDL_malloc(sizeof *font);

	if ( ! charmap_relocated ) {
		NSP_NL_RELOCDATA(nsp_font_charmaps, NSP_ARRAY_SIZE(nsp_font_charmaps));
		charmap_relocated = SDL_TRUE;
	}
	charmap = nsp_font_charmaps[font_index];
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
					&& pos.x + char_w + font->hspacing + font->char_width[(int)buffer[i + 1]] >= surface->w ) {
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
			char_w = font->char_width[(int)s[i]];
		width += char_w + font->hspacing;
	}
	if ( ! (font->flags & NSP_FONT_AUTOSIZE) )
		width -= NSP_FONT_WIDTH - font->char_width[(int)s[i - 1]];
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
	int height = NSP_FONT_HEIGHT;
	while ( *s++ )
		if ( *s == '\n' )
			height += NSP_FONT_HEIGHT + font->vspacing;
	return height;
}