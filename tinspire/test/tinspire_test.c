#include <os.h>
#include <SDL.h>
#include "../../src/video/SDL_cursor_c.h"

#define STR "Hello world\nderpdhehrhe\trhhtdepr !!!!\n  sadsaddasdah\n"

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
		NSP_FONT_TEXTWRAP);
	img = SDL_LoadBMP("Examples/image.bmp");
	SDL_nSetFontSpacing(font, 5, NSP_VSPACING_DEFAULT);
	printf("width: %d, height: %d\n", SDL_nGetStringWidth(font, STR), SDL_nGetStringHeight(font, STR));
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
	SDL_nDrawString(screen, font, NSP_COL(0), NSP_ROW(1), STR);
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