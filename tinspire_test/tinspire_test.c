#include <os.h>
#include "../include/SDL.h"

int main(void) {
    printf("SDL_Init: %d\n", SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE));
	printf("SDL_SetVideoMode: 0x%p\n", SDL_SetVideoMode(320, 240, 16, 0));
    return 0;
}