#include <os.h>
#include "../include/SDL.h"

#define RMASK	0xf800
#define GMASK	0x7e0
#define BMASK	0x1f

int main(void) {
	SDL_Surface *screen;
	SDL_Rect r1 = {10, 20, 50, 100};
	SDL_Rect r2 = {40, 30, 30, 30};
	SDL_Rect r3 = {60, 100, 200, 50};
	SDL_Surface *surf, *img;
	SDL_Joystick *joystick;
	int i;
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE))
		printf("Error: %s\n", SDL_GetError());
	screen = SDL_SetVideoMode(320, 240, 16, 0);
	SDL_JoystickEventState(SDL_ENABLE);
	puts(SDL_JoystickName(0));
	surf = SDL_CreateRGBSurface(SDL_SWSURFACE, 100, 100, 16, RMASK, GMASK, BMASK, 0);
	img = SDL_LoadBMP("Examples/image.bmp.tns");
	if(!img)
		puts("Failed to load BMP");
	puts(SDL_GetError());
	for(i = 0; i < 240; ++i) {
		SDL_Rect rect = {0, i, 320, 1};
		SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, i, i, i));
	}
	SDL_FillRect(screen, &r1, SDL_MapRGB(screen->format, 255, 0, 255));
	SDL_FillRect(screen, &r2, SDL_MapRGB(screen->format, 0, 0, 255));
	SDL_FillRect(screen, &r3, SDL_MapRGB(screen->format, 0, 255, 0));
	SDL_FillRect(surf, NULL, SDL_MapRGB(screen->format, 255, 255, 0));
	SDL_BlitSurface(surf, NULL, screen, &r3);
	//SDL_BlitSurface(img, NULL, screen, NULL); /*Doesn't work yet*/
	SDL_Flip(screen);
	//SDL_UpdateRect(screen, 20, 10, 100, 200);
	SDL_Delay(3000);
	SDL_Quit();
    return 0;
}