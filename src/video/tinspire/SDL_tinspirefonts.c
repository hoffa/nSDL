#include "SDL_config.h"
#include "../SDL_sysvideo.h"
#include "SDL_tinspirefonts.h"

#define NSP_BUF_SIZE	512

static unsigned char *nsp_font_charmaps[] = {
	nsp_font_thin,
	nsp_font_space,
	nsp_font_vga,
	nsp_font_fantasy,
	nsp_font_tinytype
};

static SDL_bool charmap_relocated = SDL_FALSE;

SDL_nFont *SDL_nLoadFont(int font_index, Uint32 color, Uint32 flags)
{
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
		int offset = 8 * i;
		int max_width = 0;
		SDL_Surface *char_surf = SDL_CreateRGBSurface(SDL_SWSURFACE, NSP_FONT_WIDTH,
			NSP_FONT_HEIGHT, NSP_BPP, NSP_RMASK, NSP_GMASK, NSP_BMASK, 0);
		if ( char_surf == NULL ) {
			SDL_OutOfMemory();
			return(NULL);
		}
		font->char_width[i] = NSP_FONT_WIDTH;
#if NSP_BPP_SW8
		if ( ! SDL_nCreatePalette(char_surf) ) {
			SDL_FreeSurface(char_surf);
			SDL_nFreeFont(font);
			return(NULL);
		}
#endif
		if ( color == 0 )
			SDL_FillRect(char_surf, NULL, 1);
		if ( SDL_SetColorKey(char_surf, SDL_SRCCOLORKEY | SDL_RLEACCEL, color ? 0 : 1) == -1 ) {
			SDL_FreeSurface(char_surf);
			SDL_nFreeFont(font);
			return(NULL);
		}
		SDL_LockSurface(char_surf);
		for ( j = 0; j < NSP_FONT_HEIGHT; ++j )
			for ( k = 0; k < NSP_FONT_WIDTH; ++k ) {
				if ( charmap[offset + j] & (1 << (NSP_FONT_WIDTH - k - 1)) ) { /* "Pixel" set */
					if ( k > max_width ) {
						font->char_width[i] = k + 1;
						max_width = k;
					}
#if NSP_BPP_SW16
					*(Uint16 *)(char_surf->pixels + (2 * k) + (2 * NSP_FONT_WIDTH * j)) = (Uint16)color;
#else
					*(Uint8 *)(char_surf->pixels + k + (NSP_FONT_WIDTH * j)) = (Uint8)color;
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

void SDL_nSetFontSpacing(SDL_nFont *font, int hspacing, int vspacing)
{
	font->hspacing = hspacing;
	font->vspacing = vspacing;
}

void SDL_nSetFontFlags(SDL_nFont *font, Uint32 flags)
{
	font->flags = flags;
}

void SDL_nFreeFont(SDL_nFont *font)
{
	int i;
	for ( i = 0; i < NSP_FONT_NUMCHARS; ++i )
		SDL_FreeSurface(font->chars[i]);
	SDL_free(font);
}

int SDL_nDrawChar(SDL_Surface *surface, SDL_nFont *font, SDL_Rect *pos, int c)
{
	SDL_Rect rect = {0, 0, 0, NSP_FONT_HEIGHT};
	rect.w = font->char_width[c];
	return(SDL_BlitSurface(font->chars[c], &rect, surface, pos));
}

/* Private. All other string drawing functions derive from this one.
   If rect->w && rect->h, the string is drawn within rect.
   Returns -1 on error, and the number of characters NOT drawn otherwise.

   Notes: NSP_FONT_TEXTWRAP only has an effect when using SDL_nDrawStringInRect(). */
static int nsp_draw_string(SDL_Surface *surface, SDL_nFont *font,
			   SDL_Rect *rect, const char *format, va_list args)
{
	char buffer[NSP_BUF_SIZE];
	SDL_Rect pos;
	int length;
	SDL_bool bounded = (rect->w && rect->h) ? SDL_TRUE : SDL_FALSE;
	int max_x = rect->x + rect->w;
	int max_y = rect->y + rect->h;
	int chars_drawn = 0;
	int i;

	vsprintf(buffer, format, args); /* Possibility of overflow if size of string > NSP_BUF_SIZE */
	length = (int)strlen(buffer);
	if ( bounded && (rect->w < 8 || rect->h < 8) )
		return(length);
	pos.x = rect->x;
	pos.y = rect->y;

	for ( i = 0; i < length; ++i ) {
		int c = buffer[i];
		int c_width = NSP_CHAR_WIDTH(font, c);
		switch ( c ) {
			case '\n':
				pos.x = rect->x;
				pos.y += NSP_FONT_HEIGHT + font->vspacing;
				++chars_drawn;
				break;
			default: {
				SDL_bool draw_char;
				if ( bounded ) {
					if ( pos.x + c_width <= max_x
					  && pos.y + NSP_FONT_HEIGHT <= max_y )
						draw_char = SDL_TRUE;
					else if ( font->flags & NSP_FONTCFG_TEXTWRAP
					       && pos.y + (2 * NSP_FONT_HEIGHT) + font->vspacing <= max_y ) {
						draw_char = SDL_TRUE;
						pos.x = rect->x;
						pos.y += NSP_FONT_HEIGHT + font->vspacing;
					} else
						draw_char = SDL_FALSE;
				} else
					draw_char = SDL_TRUE;
				if ( draw_char ) {
					if ( SDL_nDrawChar(surface, font, &pos, c) == -1 )
						return(-1);
					++chars_drawn;
				}
				if ( ! ( font->flags & NSP_FONTCFG_FORMAT ) || pos.x != rect->x || c != ' ' )
					pos.x += c_width + font->hspacing;
				}
				break;
		}
	}

	return(length - chars_drawn);
}

int SDL_nDrawString(SDL_Surface *surface, SDL_nFont *font,
		    int x, int y, const char *format, ...)
{
	va_list args;
	int ret_val;
	SDL_Rect rect = {0, 0, 0, 0};
	rect.x = x;
	rect.y = y;
	va_start(args, format);
	ret_val = nsp_draw_string(surface, font, &rect, format, args);
	va_end(args);
	return(ret_val);
}

int SDL_nDrawStringInRect(SDL_Surface *surface, SDL_nFont *font,
			  SDL_Rect *rect, const char *format, ...)
{
	va_list args;
	int ret_val;
	va_start(args, format);
	ret_val = nsp_draw_string(surface, font, rect, format, args);
	va_end(args);
	return(ret_val);
}

static int nsp_get_line_width(SDL_nFont *font, const char *s)
{
	int width = 0;
	while ( *s && *s != '\n' ) {
		if ( ! ( font->flags & NSP_FONTCFG_FORMAT ) || width || *s != ' ' )
			width += NSP_CHAR_WIDTH(font, *s) + font->hspacing;
		++s;
	}
	width -= font->hspacing;
	return((width > 0) ? width : 0);
}

int SDL_nGetStringWidth(SDL_nFont *font, const char *s)
{
	int max_width = nsp_get_line_width(font, s);
	int length = (int)strlen(s);
	int i;
	for ( i = 0; i < length; ++i )
		if ( i > 0 && s[i - 1] == '\n' ) {
			int width = nsp_get_line_width(font, &s[i]);
			if ( width > max_width )
				max_width = width;
		}
	return(max_width);
}

int SDL_nGetStringHeight(SDL_nFont *font, const char *s)
{
	int height = NSP_FONT_HEIGHT;
	while ( *s++ )
		if ( *s == '\n' )
			height += NSP_FONT_HEIGHT + font->vspacing;
	return height;
}