#ifndef _SDL_tinspirefonts_h
#define _SDL_tinspirefonts_h

#include "fonts/THIN_SS.h"
#include "fonts/SPACE8.h"
#include "fonts/VGA-ROM.h"
#include "fonts/FANTASY.h"
#include "fonts/TINYTYPE.h"

#define NSP_CHAR_WIDTH(font, c) ((font->flags & NSDL_FONTCFG_AUTOSIZE) \
				? font->char_width[(int)c] : NSP_FONT_WIDTH)
#define NSP_ASSERT(expression) do { \
	if ( ! (expression) ) \
		return; \
} while (0)
#define NSP_ASSERT_RET(expression, ret) do { \
	if ( ! (expression) ) \
		return ret; \
} while (0)

#endif /* _SDL_tinspirefonts_h */