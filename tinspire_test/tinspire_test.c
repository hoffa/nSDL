#include <os.h>
#include "../include/SDL.h"

int main(void) {
	SDL_Surface *screen, *surf;
	SDL_Rect surf_pos = {10, 20, 0, 0};
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	screen = SDL_SetVideoMode(320, 240, NSP_BPP, SDL_SWSURFACE | SDL_HWPALETTE);
	if(screen == NULL) {
		printf("SDL_SetVideoMode error: %s\n", SDL_GetError());
		return 1;
	}
	//surf = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_HWPALETTE, 100, 100, NSP_BPP, NSP_RMASK, NSP_GMASK, NSP_BMASK, 0);
	surf = SDL_LoadBMP("Examples/image.bmp.tns");
	//SDL_NSP_CreatePalette(surf);
	//SDL_FillRect(surf, NULL, SDL_MapRGB(screen->format, 255, 0, 255));
	SDL_BlitSurface(surf, NULL, screen, &surf_pos);
	//SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 0, 255));
	SDL_Flip(screen);
	SDL_Delay(3000);
	SDL_Quit();
	return 0;
}