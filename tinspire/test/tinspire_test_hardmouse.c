#include <os.h>
#include <SDL.h>

int main(void) {
	SDL_Surface *screen;
	nSDL_Font *font_vga;
	int done = 0;

	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	screen = SDL_SetVideoMode(320, 240, is_cx ? 16 : 8, SDL_SWSURFACE);
	if(screen == NULL) {
		printf("SDL_SetVideoMode error: %s\n", SDL_GetError());
		return 1;
	}
	font_vga = nSDL_LoadFont(NSDL_FONT_VGA, SDL_MapRGB(screen->format, 255, 255, 255), NSDL_FONTCFG_DEFAULT);

	while(!done) {
		SDL_Event event;
		touchpad_report_t tp;
		touchpad_scan(&tp);
		SDL_FillRect(screen, NULL, 0);
		nSDL_DrawString(screen, font_vga, 8, 8, "nSDL " NSDL_VERSION);
		nSDL_DrawString(screen, font_vga, 8, 24, "Mouse:\n Position: (%d,%d)\n Velocity: (%d,%d)\n Contact:  %d\n", tp.x, tp.y, (Sint8)tp.x_velocity, (Sint8)(-tp.y_velocity), tp.contact);
		SDL_Flip(screen);
		while(SDL_PollEvent(&event))
			if(event.type == SDL_KEYDOWN)
				done = 1;
		SDL_Delay(50);
	}

	nSDL_FreeFont(font_vga);
	SDL_Quit();

	return 0;
}