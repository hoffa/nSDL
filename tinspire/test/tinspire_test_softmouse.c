#include <os.h>
#include <SDL.h>

#define AMOUNT 10

int main(void) {
	SDL_Surface *screen;
	SDL_nFont *font_vga;
	int done = 0;
	SDL_Event event;
	SDL_Rect rect = {0, 0, 30, 30};

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
	//SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	while(!done) {
		SDL_FillRect(screen, NULL, 0);
		SDL_nDrawString(screen, font_vga, NSP_COL(1), NSP_ROW(1), NSP_NAME_FULL);
		SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 255, 255, 255));
		while(SDL_PollEvent(&event))
		switch(event.type) {
			case SDL_MOUSEMOTION:
				SDL_nDrawString(screen, font_vga, NSP_COL(1), NSP_ROW(3), "Mouse: (%d,%d)\n", event.motion.x, event.motion.y);
				break;
			case SDL_MOUSEBUTTONDOWN:
				SDL_nDrawString(screen, font_vga, NSP_COL(1), NSP_ROW(5), "SDL_MOUSEBUTTONDOWN (%d,%d)", event.button.x, event.button.y);
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
		SDL_Flip(screen);
	}
	SDL_nFreeFont(font_vga);
	SDL_Quit();

	return 0;
}