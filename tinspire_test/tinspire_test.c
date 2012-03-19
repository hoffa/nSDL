#include <os.h>
#include <SDL.h>

int main(void) {
	SDL_Surface *screen;
	SDL_nFont *font_vga;
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	screen = SDL_SetVideoMode(320, 240, NSP_BPP, SDL_SWSURFACE);
	if(screen == NULL) {
		printf("SDL_SetVideoMode error: %s\n", SDL_GetError());
		return 1;
	}
	font_vga = SDL_nLoadFont(NSP_FONT_VGA, SDL_MapRGB(screen->format, 255, 255, 255), NSP_FONT_OPAQUE);
	SDL_nDrawString(screen, font_vga, 8, 8, "Test 0x%X, %s, %d!", 65535, "TI-Nspire", 42);
	SDL_Flip(screen);
	SDL_Delay(3000);
	SDL_nFreeFont(font_vga);
	SDL_Quit();
	return 0;
}