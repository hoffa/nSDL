#include "SDL_config.h"
#include "../SDL_sysvideo.h"
#include "SDL_tinspirevideo.h"
#include "SDL_tinspirefonts.h"

nSDL_Font *nSDL_LoadFont(int font_index, Uint8 r, Uint8 g, Uint8 b)
{
	nSDL_Font *font;
	int i, j, k;

	font = SDL_malloc(sizeof(*font));
	if ( font == NULL ) {
		SDL_OutOfMemory();
		return(NULL);
	}

	for ( i = 0; i < NSP_FONT_NUMCHARS; ++i ) {
		int offset = 8 * i;
		int max_width = 0;
		Uint32 color;
		SDL_Surface *tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, NSP_FONT_WIDTH, NSP_FONT_HEIGHT,
							16, NSP_RMASK16, NSP_GMASK16, NSP_BMASK16, 0);
		if ( tmp == NULL ) {
			SDL_OutOfMemory();
			return(NULL);
		}
		color = SDL_MapRGB(tmp->format, r, g, b);
		SDL_FillRect(tmp, NULL, ! color);
		SDL_SetColorKey(tmp, SDL_SRCCOLORKEY, ! color);
		font->char_width[i] = NSP_FONT_WIDTH;
		SDL_LockSurface(tmp);
		for ( j = 0; j < NSP_FONT_HEIGHT; ++j )
			for ( k = 0; k < NSP_FONT_WIDTH; ++k ) {
				if ( nsp_font_charmaps[font_index][offset + j] & (1 << (NSP_FONT_WIDTH - k - 1)) ) { /* "Pixel" set */
					if ( k > max_width ) {
						font->char_width[i] = k + 1;
						max_width = k;
					}
					nSDL_SetPixel(tmp, k, j, color);
				}
			}
		SDL_UnlockSurface(tmp);
		font->chars[i] = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		font->hspacing = font->vspacing = 0;
		font->monospaced = SDL_FALSE;
	}

	return(font);
}

void nSDL_SetFontSpacing(nSDL_Font *font, int hspacing, int vspacing)
{
	font->hspacing = hspacing;
	font->vspacing = vspacing;
}

void nSDL_EnableFontMonospaced(nSDL_Font *font, SDL_bool toggle)
{
	font->monospaced = toggle;
}

void nSDL_FreeFont(nSDL_Font *font)
{
	int i;
	if ( font == NULL )
		return;
	for ( i = 0; i < NSP_FONT_NUMCHARS; ++i )
		SDL_FreeSurface(font->chars[i]);
	SDL_free(font);
}

int nSDL_DrawString(SDL_Surface *surface, nSDL_Font *font,
		    int x, int y, const char *format, ...)
{
	char buf[NSP_BUF_SIZE];
	int length;
	SDL_Rect pos;
	va_list args;
	int i;

	va_start(args, format);
	if ( vsprintf(buf, format, args) < 0 )
		return(-1);
	va_end(args);
	length = (int)strlen(buf);
	pos.x = x;
	pos.y = y;
	NSP_DEBUG("\"%s\" at (%d, %d)", buf, pos.x, pos.y);

	for ( i = 0; i < length; ++i ) {
		int c = buf[i];
		if ( c == '\n' ) {
			pos.x = x;
			pos.y += NSP_FONT_HEIGHT + font->vspacing;
		} else {
			SDL_Rect rect;
			rect.x = rect.y = 0;
			rect.w = font->char_width[c];
			rect.h = NSP_FONT_HEIGHT;
			if ( SDL_BlitSurface(font->chars[c], &rect, surface, &pos) == -1 )
				return(-1);
			pos.x += NSP_CHAR_WIDTH(font, c) + font->hspacing;
		}
	}

	return(0);
}

int nSDL_GetStringWidth(nSDL_Font *font, const char *s)
{
	int width = 0;
	int max_width = 0;
	do {
		if ( *s == '\n' || *s == '\0' ) {
			if ( width > max_width )
				max_width = width;
			width = 0;
		} else
			width += NSP_CHAR_WIDTH(font, *s) + font->hspacing;
	} while ( *s++ );
	return(max_width - font->hspacing);
}

int nSDL_GetStringHeight(nSDL_Font *font, const char *s)
{
	int height = 0;
	do {
		if ( *s == '\n' || *s == '\0' )
			height += NSP_FONT_HEIGHT + font->vspacing;
	} while ( *s++ );
	return(height - font->vspacing);
}