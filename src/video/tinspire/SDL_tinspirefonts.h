#ifndef _SDL_tinspirefonts_h
#define _SDL_tinspirefonts_h

#include "fonts/THIN_SS.h"
#include "fonts/SPACE8.h"
#include "fonts/VGA-ROM.h"
#include "fonts/FANTASY.h"
#include "fonts/TINYTYPE.h"

#define NSP_CHAR_WIDTH(font, c) (font->monospace ? NSP_FONT_WIDTH : font->char_width[(int)c])

#endif /* _SDL_tinspirefonts_h */