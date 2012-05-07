#include <os.h>
#include <SDL.h>

#define AMOUNT 10

int main(void) {
	SDL_Surface *screen;
	SDL_nFont *font_vga;
	int done = 0;
	SDL_Event event;
	SDL_Rect rect = {0, 0, 30, 30};
	int derp = 0;

	SDL_putenv("SDL_WARN_NOMOUSE=0");
	SDL_putenv("SDL_USEMOUSE=1");
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	screen = SDL_SetVideoMode(320, 240, 16, SDL_SWSURFACE);
	if(screen == NULL) {
		printf("SDL_SetVideoMode error: %s\n", SDL_GetError());
		return 1;
	}
	printf("#%x\n", SDL_MapRGB(screen->format, 255, 0, 255));
	font_vga = SDL_nLoadFont(NSP_FONT_TINYTYPE, SDL_MapRGB(screen->format, 1, 0, 0), NSP_FONTCFG_DEFAULT);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	while(!done) {
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 0, 255));
		//SDL_WaitEvent(&event);
		while(SDL_PollEvent(&event))
		switch(event.type) {
			case SDL_MOUSEMOTION:
				SDL_nDrawString(screen, font_vga, 10, 20, "Mouse: (%d,%d)\n", event.motion.x, event.motion.y);
				++derp;
				break;
			case SDL_MOUSEBUTTONDOWN:
				SDL_nDrawString(screen, font_vga, 10, 30, "SDL_MOUSEBUTTONDOWN (%d,%d)", event.button.x, event.button.y);
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_UP:
						rect.y -= AMOUNT;
						break;
					case SDLK_DOWN:
						rect.y += AMOUNT;
						break;
					case SDLK_LEFT:
						rect.x -= AMOUNT;
						break;
					case SDLK_RIGHT:
						rect.x += AMOUNT;
						break;
					case SDLK_ESCAPE:
						done = 1;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		SDL_FillRect(screen, &rect, 0);
		SDL_nDrawString(screen, font_vga, 10, 10, NSP_NAME_FULL);
		SDL_nDrawString(screen, font_vga, 10, 50, "%d", derp);
		SDL_Flip(screen);
	}
	SDL_nFreeFont(font_vga);
	SDL_Quit();

	return 0;
}