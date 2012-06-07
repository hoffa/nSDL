#include <os.h>
#include <SDL.h>

#define NUM_BLITS	10000

int main(void) {
	SDL_Surface *screen, *tmp, *image;
	nSDL_Font *font_vga;
	Uint32 start, end;
	SDL_Rect pos;
	int max_x;
	int incdec;
	int i;

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
	tmp = SDL_LoadBMP("Examples/image.bmp.tns");
	if(tmp == NULL) {
		printf("SDL_LoadBMP error: %s\n", SDL_GetError());
		return 1;
	}
	image = SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);
	incdec = 1;
	max_x = SCREEN_WIDTH - image->w;
	pos.x = 0;
	pos.y = 0;
	SDL_FillRect(screen, NULL, 0);
	start = SDL_GetTicks();
	puts("START");
	for(i = 0; i < NUM_BLITS; ++i) {
		SDL_BlitSurface(image, NULL, screen, &pos);
		SDL_UpdateRect(screen, pos.x, pos.y, image->w, image->h);
		pos.x += incdec;
		if(pos.x == 0)
			incdec = 1;
		else if(pos.x == max_x)
			incdec = -1;
	}
	puts("END");
	end = SDL_GetTicks();
	nSDL_DrawString(screen, font_vga, 0, 0, "%s\n\n%d blits in %u ms", "nSDL " NSDL_VERSION, NUM_BLITS, (unsigned int)(end - start));
	SDL_Flip(screen);
	SDL_Delay(5000);
	nSDL_FreeFont(font_vga);
	SDL_FreeSurface(image);
	SDL_Quit();

	return 0;
}