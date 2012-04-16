#include <os.h>
#include <SDL.h>

int main(void) {
	SDL_Surface *screen;
	SDL_nFont *font_vga;
	int done = 0;
	SDL_Event event;

	SDL_putenv("SDL_WARN_NOMOUSE=1");
	SDL_putenv("SDL_NOMOUSE=1");
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	screen = SDL_SetVideoMode(320, 240, NSP_BPP, SDL_SWSURFACE);
	if(screen == NULL) {
		printf("SDL_SetVideoMode error: %s\n", SDL_GetError());
		return 1;
	}
	font_vga = SDL_nLoadFont(NSP_FONT_VGA, SDL_MapRGB(screen->format, 255, 255, 255), NSP_FONT_DEFAULT);
	while(!done) {
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 24, 46, 103));
		SDL_nDrawString(screen, font_vga, NSP_COL(1), NSP_ROW(1), NSP_NAME_FULL);
		SDL_WaitEvent(&event);
		switch(event.type) {
			case SDL_MOUSEMOTION:
				SDL_nDrawString(screen, font_vga, NSP_COL(1), NSP_ROW(3), "Mouse: (%d,%d)\n", event.motion.x, event.motion.y);
				break;
			case SDL_MOUSEBUTTONDOWN:
				SDL_nDrawString(screen, font_vga, NSP_COL(1), NSP_ROW(5), "SDL_MOUSEBUTTONDOWN (%d,%d)", event.button.x, event.button.y);
				break;
			case SDL_KEYUP:
				done = 1;
				break;
			default:
				break;
		}
		SDL_Flip(screen);
	}
	SDL_nFreeFont(font_vga);
	SDL_Quit();

	return 0;
}