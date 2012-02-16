#include <os.h>
#include "../include/SDL.h"

int main(void) {
	SDL_Surface *screen;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
	screen = SDL_SetVideoMode(320, 240, 16, 0);
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 0, 255));
	SDL_Flip(screen);
	sleep(3000);
	SDL_Quit();
    return 0;
}