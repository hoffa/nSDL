#ifndef _SDL_tinspirenti_h
#define _SDL_tinspirenti_h

#include "SDL_stdinc.h"

/* NTI specification: https://github.com/Hoffa/nSDL/wiki/NTI-specification */

typedef struct nti_info_t {
	Uint8 magic;
	Uint8 version;
	Uint16 width, height;
	Uint16 reserved;
} nti_info_t;

#endif /* _SDL_tinspirenti_h */