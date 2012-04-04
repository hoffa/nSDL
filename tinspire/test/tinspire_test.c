#include <os.h>
#include <SDL.h>
#include "../../src/video/SDL_cursor_c.h"

int main(void) {
	SDL_Surface *screen, *img;
	SDL_nFont *font;
	int done = 0;
	int i;
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	screen = SDL_SetVideoMode(320, 240, NSP_BPP, SDL_SWSURFACE);
	if(screen == NULL) {
		printf("SDL_SetVideoMode error: %s\n", SDL_GetError());
		return 1;
	}
	font = SDL_nLoadFont(NSP_FONT_THIN, SDL_MapRGB(screen->format, 255, 255, 255),
		NSP_FONT_TEXTWRAP | NSP_FONT_AUTOSIZE);
	img = SDL_LoadBMP("Examples/image.bmp");
	SDL_nSetFontSpacing(font, 5, NSP_VSPACING_DEFAULT);
	printf("width: %d, height: %d\n", SDL_nGetLineWidth(font, "Hello, world..d\n!\0\n"), SDL_nGetStringHeight(font, "Hello, world!"));
	SDL_FillRect(screen, NULL, 0);
	for(i = 0; i < 200; ++i) {
		SDL_Rect rect;
		rect.x = i;
		rect.y = 0;
		rect.w = 1;
		rect.h = 240;
		SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, i, i, i));
	}
	SDL_BlitSurface(img, NULL, screen, NULL);
	SDL_nDrawString(screen, font, NSP_COL(0), NSP_ROW(1),
		"The quick brown fox jumps over the lazy dogg\nish");
	SDL_nDrawString(screen, font, NSP_COL(0), NSP_ROW(5), "Hello, world..d\n!");
	SDL_Flip(screen);
	SDL_Delay(5000);
#if 0
	SDL_DrawCursor(screen);
	while(!done) {
		SDL_Event event;
		SDL_FillRect(screen, NULL, 0);
		//SDL_nDrawString(screen, font, NSP_COL(1), NSP_ROW(1), NSP_NAME_FULL);
		//while(SDL_PollEvent(&event))
		SDL_WaitEvent(&event);
		if(event.key.keysym.sym == SDLK_ESCAPE)
			done = 1;
		SDL_nDrawString(screen, font, NSP_COL(5), NSP_ROW(5), "(%d,%d)", SDL_cursor->area.x, SDL_cursor->area.y);
		SDL_Flip(screen);
	}
#endif
	SDL_FreeSurface(img);
	SDL_nFreeFont(font);
	SDL_Quit();
	return 0;
}