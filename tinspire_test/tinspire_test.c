#include <os.h>
#include "../include/SDL.h"

int main(void) {
	SDL_Surface *screen, *image;
	SDL_Rect image_pos = {10, 20, 0, 0};
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	screen = SDL_SetVideoMode(320, 240, NSP_BPP, SDL_SWSURFACE | SDL_HWPALETTE);
	if(screen == NULL) {
		printf("SDL_SetVideoMode error: %s\n", SDL_GetError());
		return 1;
	}
	image = SDL_DisplayFormat(SDL_LoadBMP("Examples/image.bmp.tns"));
	SDL_BlitSurface(image, NULL, screen, &image_pos);
	SDL_Flip(screen);
	SDL_Delay(1000);
	SDL_Quit();
	return 0;
}