#include <os.h>
#include <SDL.h>

int main(void) {
	SDL_Surface *screen, *image;
	SDL_nFont *font_vga;
	int done = 0;
	touchpad_report_t *tp = malloc(sizeof *tp);

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
	image = SDL_LoadBMP("Examples/image.bmp.tns");
	while(!done) {
		SDL_Event event;
		SDL_FillRect(screen, NULL, 0);
		touchpad_scan(tp);
		SDL_nDrawString(screen, font_vga, NSP_COL(1), NSP_ROW(1), "nSDL " NSP_VERSION " test program");
		SDL_nDrawString(screen, font_vga, NSP_COL(1), NSP_ROW(3), "Mouse:\n Position: %d\t%d\n Velocity: %d\t%d\n Contact:  %d\n", tp->x, tp->y, (Sint8)tp->x_velocity, (Sint8)tp->y_velocity, tp->contact);
		SDL_BlitSurface(image, NULL, screen, NULL);
		SDL_Flip(screen);
		while(SDL_PollEvent(&event))
			if(event.type == SDL_KEYDOWN)
				done = 1;
		SDL_Delay(100);
	}
	SDL_nFreeFont(font_vga);
	SDL_Quit();

	return 0;
}