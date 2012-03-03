#include <os.h>
#include "../include/SDL.h"

int main(void) {
	SDL_Surface *screen;
	printf("SDL_Init: %d\n", SDL_Init(SDL_INIT_VIDEO));
	screen = SDL_SetVideoMode(320, 240, 8, 0);
	if(!screen)
		puts(SDL_GetError());
	/*printf("255,255,255: %d\n", SDL_MapRGB(screen->format, 255, 255, 255));
	printf("255,0,255: %d\n", SDL_MapRGB(screen->format, 255, 0, 255));
	printf("0,0,255: %d\n", SDL_MapRGB(screen->format, 0, 0, 255));
	printf("255,255,0: %d\n", SDL_MapRGB(screen->format, 255, 255, 0));
	printf("255,0,0: %d\n", SDL_MapRGB(screen->format, 255, 0, 0));*/
	//SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
	//SDL_Flip(screen);
	SDL_Quit();
	return 0;
}