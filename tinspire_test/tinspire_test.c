#include <os.h>
#include "../include/SDL.h"

int main(void) {
	SDL_Surface *screen, *surf;
	SDL_Rect r = {10, 20, 0, 0};
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
	surf = SDL_CreateRGBSurface(SDL_SWSURFACE, 60, 50, NSP_BPP, NSP_RMASK, NSP_BMASK, NSP_GMASK, 0);
	if(surf == NULL) {
		printf("SDL_CreateRGBSurface error: %s\n", SDL_GetError());
		return 1;
	}
	//nsp_create_palette(surf);
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	printf("SDL_FillRect: %d\n", SDL_FillRect(surf, NULL, SDL_MapRGB(screen->format, 255, 0, 255)));
	//printf("SDL_BlitSurface: %d\n", SDL_BlitSurface(surf, NULL, screen, NULL)); /* does not work */
	SDL_Flip(screen);
	SDL_Delay(1000);
	printf("%d\n", SDL_GetTicks());
	SDL_Delay(3000);
	//SDL_FreeSurface(surf);
	printf("%d\n", SDL_GetTicks());
	SDL_Quit();
	return 0;
}