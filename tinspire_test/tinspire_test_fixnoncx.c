#include <os.h>
#include "../include/SDL.h"

/* I am not responsible for any damage caused by this code to your eyes */
int main(void) {
	SDL_Surface *screen;
	SDL_Rect r1 = {10, 20, 10, 10};
	SDL_Rect r2 = {40, 30, 30, 30};
	SDL_Rect r3 = {310, 230, 10, 10};
	SDL_Surface *surf, *img;
	SDL_Joystick *joystick;
	SDL_Event event;
	int quit = 0;
	int i;
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0)
		printf("Error: %s\n", SDL_GetError());
	screen = SDL_SetVideoMode(320, 240, NSP_BPP, 0);
	//SDL_JoystickEventState(SDL_ENABLE);
	//printf("joystickname: %s\n", SDL_JoystickName(0));
	//joystick = SDL_JoystickOpen(0);
	//img = SDL_LoadBMP("Examples/image.bmp.tns");
	//if(!img)
	//	puts("Failed to load BMP");
	//puts(SDL_GetError());
	//printf("flags: 0x%x, size: %dx%d, pitch: %d, refcount: %d, bipp: %d\n", img->flags, img->w, img->h, img->pitch, img->refcount, img->format->BitsPerPixel);
	for(i = 0; i < 240; ++i) {
		SDL_Rect rect = {0, i, 320, 1};
		SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, i, i, i));
	}
	SDL_FillRect(screen, &r2, SDL_MapRGB(screen->format, 0, 0, 255));
	SDL_FillRect(screen, &r3, SDL_MapRGB(screen->format, 0, 255, 0));
	printf("main(void) color: %d\n", SDL_MapRGB(screen->format, 240, 240, 240));
	printf("pixel (0,0): %d\n", *(unsigned char *)surf->pixels);
	printf("blit: %d\n", SDL_BlitSurface(surf, NULL, screen, NULL));
	surf = SDL_CreateRGBSurface(SDL_SWSURFACE, 100, 100, NSP_BPP, NSP_RMASK, NSP_GMASK, NSP_BMASK, 0);
	printf("fillrect: %d\n", SDL_FillRect(surf, NULL, SDL_MapRGB(screen->format, 240, 240, 240)));
	printf("blit: %d\n", SDL_BlitSurface(surf, NULL, screen, &r2)); /* fails if r2 set, nothing if NULL (on non-CX) */
	//printf("blit: %d\n", SDL_BlitSurface(img, NULL, screen, NULL)); /*Doesn't work yet*/
	//SDL_UpdateRect(screen, 20, 10, 100, 200);
	while(!quit) {
		r1.w = r1.h = 10;
		SDL_FillRect(screen, &r1, SDL_MapRGB(screen->format, 255, 0, 255));
		printf("x: %d, y: %d\n", r1.x, r1.y);
		SDL_Flip(screen);
		SDL_WaitEvent(&event);
		switch(event.type) {
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_ESCAPE:
						quit = 1;
						break;
					case SDLK_UP:
						--r1.y;
						break;
					case SDLK_DOWN:
						++r1.y;
						break;
					case SDLK_LEFT:
						--r1.x;
						break;
					case SDLK_RIGHT:
						++r1.x;
						break;
					default:
						break;
				}
				break;
			default:
				break;
			/*case SDL_JOYBUTTONDOWN:
				printf("Button down: %d\n", event.jbutton.button);
				if(event.jbutton.button == NSP_JB_ESC) {
					puts("NSP_JB_ESC pressed, quitting");
					quit = 1;
				}
				break;
			case SDL_JOYBUTTONUP:
				printf("Button up: %d\n", event.jbutton.button);
				break;
			case SDL_JOYAXISMOTION:
				printf("Axis: %d, value: %d\n", event.jaxis.axis, event.jaxis.value);
				switch(event.jaxis.axis) {
					case NSP_JA_H:
						r1.x += event.jaxis.value;
						break;
					case NSP_JA_V:
						r1.y += event.jaxis.value;
						break;
					default:
						break;
				}
				break;
			default:
				break;*/
		}
		SDL_Delay(10);
	}
	SDL_Quit();
    return 0;
}