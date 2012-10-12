#include "SDL_video.h"
#include "SDL_stdinc.h"
#include "SDL_tinspirevideo.h"
#include "SDL_tinspirenti.h"

static void nti_get_info(nti_info_t *nti_info, Uint16 *data)
{
	nti_info->magic = data[0] >> 8;
	nti_info->version = data[0] & 0x00ff;
	nti_info->width = data[1];
	nti_info->height = data[2];
	nti_info->reserved = data[3];
	return(nti_info);
}

SDL_Surface *nSDL_LoadImage(Uint16 *data)
{
	SDL_Surface *tmp, *image;
	nti_info_t nti_info;
	int i, j;
	nti_get_info(&nti_info, data);
	if ( nti_info.magic != 42 ) {
		SDL_SetError("[NSP] Invalid NTI image");
		return(NULL);
	}
	NSP_DEBUG("Loading NTI v%d (%dx%d)", nti_info.version, nti_info.width, nti_info.height);
	tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, nti_info.width, nti_info.height,
				   16, NSP_RMASK16, NSP_GMASK16, NSP_BMASK16, 0);
	if ( tmp == NULL ) {
		SDL_OutOfMemory();
		return(NULL);
	}
	data = (Uint16 *)(data + 4);
	SDL_LockSurface(tmp);
	for ( i = 0; i < nti_info.height; ++i )
		for( j = 0; j < nti_info.width; ++j)
			*(Uint16 *)NSP_SURF_PIXEL(tmp, j, i) = data[j + (nti_info.width * i)];
	SDL_UnlockSurface(tmp);
	image = SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);
	return(image);
}