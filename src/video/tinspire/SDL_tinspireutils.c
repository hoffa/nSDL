#include "SDL_video.h"
#include "SDL_tinspirevideo.h"

__inline__ Uint32 nSDL_GetPixel(SDL_Surface *surface, int x, int y)
{
	Uint8 *pixel = NSP_SURF_PIXEL(surface, x, y);
	switch ( surface->format->BytesPerPixel ) {
		case 1: return *pixel;
		case 2: return *(Uint16 *)pixel;
		case 3: return pixel[0] | (pixel[1] << 8) | (pixel[2] << 16);
		case 4: return *(Uint32 *)pixel;
		default: return 0;
	}
}

__inline__ void nSDL_SetPixel(SDL_Surface *surface, int x, int y, Uint32 color)
{
	Uint8 *pixel = NSP_SURF_PIXEL(surface, x, y);
	switch ( surface->format->BytesPerPixel ) {
		case 1:
			*pixel = color;
			break;
		case 2:
			*(Uint16 *)pixel = color;
			break;
		case 3:
			pixel[0] = color & 0xff;
			pixel[1] = (color >> 8) & 0xff;
			pixel[2] = (color >> 16) & 0xff;
			break;
		case 4:
			*(Uint32 *)pixel = color;
			break;
		default:
			break;
	}
}