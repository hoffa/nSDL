#include <os.h>
#include "../include/SDL.h"

int main(void) {
	SDL_Surface *screen;
	SDL_Rect r1 = {10, 20, 30, 40};
	int i;
	int quit = 0;
	SDL_Event event;
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	screen = SDL_SetVideoMode(320, 240, NSP_BPP, SDL_SWSURFACE | SDL_HWPALETTE);
	if(screen == NULL) {
		printf("SDL_SetVideoMode error: %s\n", SDL_GetError());
		return 1;
	}
	for(i = 0; i < 240; ++i) {
		SDL_Rect rect = {0, i, 320, 1};
		SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, i, i, i));
	}
	SDL_FillRect(screen, &r1, SDL_MapRGB(screen->format, 255, 0, 255));
	SDL_Flip(screen);
	while(!quit) {
		SDL_WaitEvent(&event);
		switch(event.type) {
			case SDL_KEYDOWN:
				quit = 1;
				break;
			default:
				break;
		}
	}
	SDL_Quit();
	return 0;
}