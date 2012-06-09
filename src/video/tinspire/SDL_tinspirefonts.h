#ifndef _SDL_tinspirefonts_h
#define _SDL_tinspirefonts_h

#include "fonts/THIN_SS.h"
#include "fonts/SPACE8.h"
#include "fonts/VGA-ROM.h"
#include "fonts/FANTASY.h"
#include "fonts/TINYTYPE.h"

#define NSP_CHAR_WIDTH(font, c) ((font->flags & NSDL_FONTCFG_AUTOSIZE) \
				? font->char_width[(int)c] : NSP_FONT_WIDTH)

#endif /* _SDL_tinspirefonts_h */