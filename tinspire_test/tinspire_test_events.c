#include <os.h>
#include "../include/SDL.h"

int main(void) {
	SDL_Surface *screen;
	SDL_Event event;
	SDL_Rect kd = {1, 1, 10, 10};
	SDL_Rect ku = {12, 1, 10, 10};
	SDL_Rect ne = {24, 1, 10, 10};
	int quit = 0;
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	screen = SDL_SetVideoMode(320, 240, NSP_BPP, SDL_SWSURFACE | SDL_HWPALETTE);
	if(screen == NULL) {
		printf("SDL_SetVideoMode error: %s\n", SDL_GetError());
		return 1;
	}
	while(!quit) {
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		if(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN:
					puts("SDL_KEYDOWN");
					SDL_FillRect(screen, &kd, SDL_MapRGB(screen->format, 255, 0, 0));
					if(event.key.keysym.sym == SDLK_ESCAPE)
						quit = 1;
					break;
				case SDL_KEYUP:
					puts("SDL_KEYUP");
					SDL_FillRect(screen, &ku, SDL_MapRGB(screen->format, 0, 255, 0));
					break;
				default:
					puts("No event");
					SDL_FillRect(screen, &ne, SDL_MapRGB(screen->format, 0, 0, 255));
					break;
			}
			SDL_Flip(screen);
		}
	}
	SDL_Quit();
	return 0;
}