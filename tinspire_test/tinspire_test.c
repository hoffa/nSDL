#include <os.h>
#include "../include/SDL.h"

int main(void) {
    printf("SDL_Init: %d\n", SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE));
    return 0;
}