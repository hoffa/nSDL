#include <os.h>
#include <SDL.h>

#define AMOUNT 10

int main(void) {
	SDL_Surface *screen;
	nSDL_Font *font_vga;
	int done = 0;
	SDL_Event event;
	SDL_Rect rect = {0, 0, 30, 30};
	int derp = 0;

	SDL_putenv("NSDL_WARN_NOMOUSE=1");
	SDL_putenv("NSDL_USEMOUSE=1");
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	screen = SDL_SetVideoMode(320, 240, is_cx ? 16 : 8, SDL_SWSURFACE);
	if(screen == NULL) {
		printf("SDL_SetVideoMode error: %s\n", SDL_GetError());
		return 1;
	}
	printf("#%x\n", SDL_MapRGB(screen->format, 255, 0, 255));
	font_vga = nSDL_LoadFont(NSDL_FONT_TINYTYPE, SDL_MapRGB(screen->format, 1, 0, 0), NSDL_FONTCFG_DEFAULT);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	while(!done) {
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 0, 255));
		//SDL_WaitEvent(&event);
		while(SDL_PollEvent(&event))
		switch(event.type) {
			case SDL_MOUSEMOTION:
				nSDL_DrawString(screen, font_vga, 10, 20, "Mouse: (%d,%d)\n", event.motion.x, event.motion.y);
				++derp;
				break;
			case SDL_MOUSEBUTTONDOWN:
				nSDL_DrawString(screen, font_vga, 10, 30, "SDL_MOUSEBUTTONDOWN (%d,%d)", event.button.x, event.button.y);
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
		nSDL_DrawString(screen, font_vga, 10, 10, "nSDL " NSDL_VERSION);
		nSDL_DrawString(screen, font_vga, 10, 50, "%d", derp);
		SDL_Flip(screen);
	}
	nSDL_FreeFont(font_vga);
	SDL_Quit();

	return 0;
}