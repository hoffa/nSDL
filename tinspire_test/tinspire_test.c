#include <os.h>
#include "../include/SDL.h"

#define AMOUNT 10

int main(void) {
	SDL_Surface *screen, *img;
	SDL_nFont *font_space, *font_tinytype;
	SDL_Rect rect = {0, 0, 0, 0};
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	screen = SDL_SetVideoMode(320, 240, NSP_BPP, SDL_SWSURFACE);
	if(screen == NULL) {
		printf("SDL_SetVideoMode error: %s\n", SDL_GetError());
		return 1;
	}
	font_space = SDL_nLoadFont(NSP_FONT_THIN, SDL_MapRGB(screen->format, 101, 190, 58), NSP_FONT_OPAQUE | NSP_FONT_TEXTWRAP);
	font_tinytype = SDL_nLoadFont(NSP_FONT_TINYTYPE, SDL_MapRGB(screen->format, 80, 145, 184), NSP_FONT_DEFAULT);
	img = SDL_LoadBMP("Examples/image.bmp.tns");
	SDL_FillRect(screen, NULL, 0);
	SDL_BlitSurface(img, NULL, screen, &rect);
	SDL_nDrawString(screen, "SDL on the TI-Nspire, oh boy!", 10, 10, font_tinytype);
	SDL_nDrawString(screen, "Damndsdddd\twhat a\nsexsdds\tajksdjkashdashdkjashdjkashkjdhaklfdjsf jdkfjsdkfj kldsfkjdf sfjd khsdhasjdhajsdhjhjhfont.", 8, 20, font_space);
	SDL_Flip(screen);
	SDL_Delay(3000);
	SDL_nFreeFont(font_space);
	SDL_nFreeFont(font_tinytype);
	SDL_Quit();
	return 0;
}