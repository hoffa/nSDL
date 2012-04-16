#include <os.h>
#include <SDL.h>

int main(void) {
	SDL_Surface *screen;
	SDL_nFont *font_vga;
	int done = 0;

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
		SDL_Event event;
		touchpad_report_t tp;
		touchpad_scan(&tp);
		SDL_FillRect(screen, NULL, 0);
		SDL_nDrawString(screen, font_vga, NSP_COL(1), NSP_ROW(1), NSP_NAME_FULL);
		SDL_nDrawString(screen, font_vga, NSP_COL(1), NSP_ROW(3), "Mouse:\n Position: (%d,%d)\n Velocity: (%d,%d)\n Contact:  %d\n", tp.x, tp.y, (Sint8)tp.x_velocity, (Sint8)(-tp.y_velocity), tp.contact);
		SDL_Flip(screen);
		while(SDL_PollEvent(&event))
			if(event.type == SDL_KEYDOWN)
				done = 1;
		SDL_Delay(50);
	}

	SDL_nFreeFont(font_vga);
	SDL_Quit();

	return 0;
}