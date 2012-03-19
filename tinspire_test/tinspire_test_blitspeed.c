#include <os.h>
#include <SDL.h>

#define NUM_BLITS	1000

#define RAND_RANGE(min, max) ((random() % (max - min + 1)) + min)

unsigned int random(void) {
	static unsigned int u = 1234, v = 1234;
	u = (18000 * (u & 0xFFFF)) + (u >> 16);
	v = (36969 * (v & 0xFFFF)) + (v >> 16);
	return (v << 16) + u;
}

int main(void) {
	SDL_Surface *screen, *tmp, *image;
	SDL_nFont *font_vga;
	Uint32 start, end;
	int i;
	int rand_wmax, rand_hmax;
	char buf[256];
	SDL_Rect pos;

	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	screen = SDL_SetVideoMode(320, 240, NSP_BPP, SDL_SWSURFACE);
	if(screen == NULL) {
		printf("SDL_SetVideoMode error: %s\n", SDL_GetError());
		return 1;
	}
	font_vga = SDL_nLoadFont(NSP_FONT_VGA, SDL_MapRGB(screen->format, 255, 0, 255), NSP_FONT_OPAQUE);
	tmp = SDL_LoadBMP("Examples/image.bmp.tns");
	if(tmp == NULL) {
		printf("SDL_LoadBMP error: %s\n", SDL_GetError());
		return 1;
	}
	image = SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);
	rand_wmax = 320 - image->w;
	rand_hmax = 240 - image->h;
	SDL_FillRect(screen, NULL, 0);
	start = SDL_GetTicks();
	for(i = 0; i < NUM_BLITS; ++i) {
		pos.x = RAND_RANGE(0, rand_wmax);
		pos.y = RAND_RANGE(0, rand_hmax);
		SDL_BlitSurface(image, NULL, screen, &pos);
		SDL_UpdateRect(screen, pos.x, pos.y, image->w, image->h);
	}
	end = SDL_GetTicks();
	sprintf(buf, "%d blits in %u ms", NUM_BLITS, (unsigned int)(end - start));
	SDL_nDrawString(screen, buf, 10, 10, font_vga);
	SDL_Flip(screen);
	SDL_Delay(10000);
	SDL_nFreeFont(font_vga);
	SDL_FreeSurface(image);
	SDL_Quit();

	return 0;
}