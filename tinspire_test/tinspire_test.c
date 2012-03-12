#include <os.h>
#include "../include/SDL.h"

#define AMOUNT 10

int main(void) {
	SDL_Surface *screen, *surf, *surf_fast;
	SDL_Rect surf_pos = {10, 20, 0, 0};
	int quit = 0, update = 1;
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	screen = SDL_SetVideoMode(320, 240, NSP_BPP, SDL_SWSURFACE | SDL_HWPALETTE);
	if(screen == NULL) {
		printf("SDL_SetVideoMode error: %s\n", SDL_GetError());
		return 1;
	}
	surf = SDL_LoadBMP("Examples/image.bmp.tns");
	surf_fast = SDL_DisplayFormat(surf);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	while(!quit) {
		SDL_Event event;
		if(update) {
			SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
			SDL_BlitSurface(surf_fast, NULL, screen, &surf_pos);
			SDL_NSP_DrawString(screen, "Default font", 10, 20, NSP_FONT_DEFAULT, SDL_MapRGB(screen->format, 255, 0, 255));
			SDL_NSP_DrawString(screen, "VGA font", 10, 30, NSP_FONT_VGA, SDL_MapRGB(screen->format, 255, 255, 255));
			SDL_NSP_DrawString(screen, "Space font", 10, 40, NSP_FONT_SPACE, SDL_MapRGB(screen->format, 0, 0, 255));
			SDL_NSP_DrawString(screen, "Fantasy font", 10, 50, NSP_FONT_FANTASY, SDL_MapRGB(screen->format, 100, 200, 50));
			SDL_Flip(screen);
		}
		SDL_WaitEvent(&event);
		switch(event.type) {
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_UP:
						surf_pos.y -= AMOUNT;
						break;
					case SDLK_DOWN:
						surf_pos.y += AMOUNT;
						break;
					case SDLK_LEFT:
						surf_pos.x -= AMOUNT;
						break;
					case SDLK_RIGHT:
						surf_pos.x += AMOUNT;
						break;
					case SDLK_ESCAPE:
						quit = 1;
						break;
					default:
						break;
				}
				update = 1;
				break;
			default:
				update = 0;
				break;
		}
	}
	SDL_FreeSurface(surf_fast);
	SDL_FreeSurface(surf);
	SDL_Quit();
	return 0;
}