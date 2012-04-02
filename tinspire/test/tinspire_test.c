#include <os.h>
#include <SDL.h>
#include "../../src/video/SDL_cursor_c.h"

int main(void) {
	SDL_Surface *screen;
	SDL_nFont *font;
	int done = 0;
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	screen = SDL_SetVideoMode(320, 240, NSP_BPP, SDL_SWSURFACE);
	if(screen == NULL) {
		printf("SDL_SetVideoMode error: %s\n", SDL_GetError());
		return 1;
	}
	font = SDL_nLoadFont(NSP_FONT_THIN, SDL_MapRGB(screen->format, 255, 0, 255),
		NSP_FONT_TEXTWRAP | NSP_FONT_AUTOSIZE);
	SDL_nSetFontSpacing(font, 0, NSP_VSPACING_DEFAULT);
	SDL_FillRect(screen, NULL, 0);
	SDL_nDrawString(screen, font, NSP_COL(0), NSP_ROW(1),
		"The quick brown fox jumps over the lazy dog.");
	SDL_Flip(screen);
	SDL_Delay(5000);
#if 0
	SDL_DrawCursor(screen);
	while(!done) {
		SDL_Event event;
		SDL_FillRect(screen, NULL, 0);
		//SDL_nDrawString(screen, font, NSP_COL(1), NSP_ROW(1), NSP_NAME_FULL);
		//while(SDL_PollEvent(&event))
		SDL_WaitEvent(&event);
		if(event.key.keysym.sym == SDLK_ESCAPE)
			done = 1;
		SDL_nDrawString(screen, font, NSP_COL(5), NSP_ROW(5), "(%d,%d)", SDL_cursor->area.x, SDL_cursor->area.y);
		SDL_Flip(screen);
	}
#endif
	SDL_nFreeFont(font);
	SDL_Quit();
	return 0;
}