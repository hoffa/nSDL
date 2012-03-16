#include <os.h>
#include "../include/SDL.h"

#define AMOUNT 10

int main(void) {
	SDL_Surface *screen;
	int quit = 0;
	SDL_nFont *font_space;
	SDL_nFont *font_vga;
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	screen = SDL_SetVideoMode(320, 240, NSP_BPP, SDL_SWSURFACE | SDL_HWPALETTE);
	if(screen == NULL) {
		printf("SDL_SetVideoMode error: %s\n", SDL_GetError());
		return 1;
	}
	font_space = SDL_nLoadFont(NSP_FONT_SPACE, SDL_MapRGB(screen->format, 255, 0, 255));
	font_vga = SDL_nLoadFont(NSP_FONT_TINYTYPE, SDL_MapRGB(screen->format, 255, 255, 255));
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, 1000);
	while(!quit) {
		SDL_Event event;
		char buf[32];
		sprintf(buf, "Timer: %u", (unsigned)SDL_GetTicks());
		SDL_FillRect(screen, NULL, 0);
		SDL_nDrawString(screen, "TI-Nspire SDL", 10, 10, font_vga);
		SDL_nDrawString(screen, buf, 10, 20, font_space);
		SDL_Flip(screen);
		SDL_WaitEvent(&event);
		switch(event.type) {
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE)
					quit = 1;
				break;
			default:
				break;
		}
	}
	SDL_nFreeFont(font_space);
	SDL_nFreeFont(font_vga);
	SDL_Quit();
	return 0;
}