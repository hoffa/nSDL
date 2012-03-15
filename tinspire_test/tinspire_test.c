#include <os.h>
#include "../include/SDL.h"

#define AMOUNT 10

int main(void) {
	SDL_Surface *screen;
	int quit = 0;
	SDL_NSP_Font *font_space;
	SDL_NSP_Font *font_vga;
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	screen = SDL_SetVideoMode(320, 240, NSP_BPP, SDL_SWSURFACE | SDL_HWPALETTE);
	if(screen == NULL) {
		printf("SDL_SetVideoMode error: %s\n", SDL_GetError());
		return 1;
	}
	font_space = SDL_NSP_LoadFont(NSP_FONT_SPACE, SDL_MapRGB(screen->format, 255, 0, 255));
	font_vga = SDL_NSP_LoadFont(NSP_FONT_VGA, SDL_MapRGB(screen->format, 255, 255, 255));
	while(!quit) {
		SDL_Event event;
		SDL_NSP_DrawString(screen, "Hello world!", 10, 20, font_space);
		SDL_NSP_DrawString(screen, "What's up niggra?", 10, 30, font_vga);
		SDL_Flip(screen);
		SDL_WaitEvent(&event);
		switch(event.type) {
			case SDL_KEYDOWN:
				quit = 1;
				break;
			default:
				break;
		}
	}
	SDL_NSP_FreeFont(font_space);
	SDL_NSP_FreeFont(font_vga);
	SDL_Quit();
	return 0;
}