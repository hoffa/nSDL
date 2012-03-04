#include <os.h>
#include "../include/SDL.h"

int main(void) {
	puts("Begin");
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 0;
	}
	if(SDL_SetVideoMode(320, 240, NSP_BPP, SDL_SWSURFACE | SDL_HWPALETTE) == NULL) {
		printf("SDL_SetVideoMode error: %s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}
	SDL_Quit();
	puts("End");
	return 0;
}