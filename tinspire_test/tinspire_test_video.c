#include <os.h>
#include "../include/SDL.h"

#define MOVE_AMOUNT 10

int main(void) {
	SDL_Surface *screen;
	SDL_Rect r = {1, 1, 10, 10};
	SDL_Event event;
	int quit = 0;
	int i;
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
	while(!quit) {
		//SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, 255, 0, 255));
		SDL_Flip(screen);
		//SDL_UpdateRect(screen, 3, 1, 99, 99);
		SDL_WaitEvent(&event);
		switch(event.type) {
				switch(event.key.keysym.sym) {
					case SDLK_ESCAPE:
						quit = 1;
						break;
					case SDLK_UP:
						r.y -= MOVE_AMOUNT;
						break;
					case SDLK_DOWN:
						r.y += MOVE_AMOUNT;
						break;
					case SDLK_LEFT:
						r.x -= MOVE_AMOUNT;
						break;
					case SDLK_RIGHT:
						r.x += MOVE_AMOUNT;
						break;
					default:
						break;
				}
			default:
				break;
#endif
		}
	}
	SDL_Quit();
	return 0;
}