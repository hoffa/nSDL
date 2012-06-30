#include "SDL_video.h"
#include "SDL_stdinc.h"
#include "SDL_tinspirevideo.h"
#include "SDL_tinspirenti.h"

static nti_info_t *nti_get_info(Uint16 *data)
{
	nti_info_t *nti_info = SDL_malloc(sizeof(*nti_info));
	if ( nti_info == NULL ) {
		SDL_OutOfMemory();
		return(NULL);
	}
	nti_info->magic = data[0] >> 8;
	nti_info->version = data[0] & 0x00ff;
	nti_info->width = data[1];
	nti_info->height = data[2];
	nti_info->reserved = data[3];
	return(nti_info);
}

static void nti_free_info(nti_info_t *nti_info)
{
	SDL_free(nti_info);
}

SDL_Surface *nSDL_LoadImage(Uint16 *data)
{
	SDL_Surface *tmp, *image;
	nti_info_t *nti_info = nti_get_info(data);
	int i, j;
	if ( nti_info == NULL ) {
		SDL_OutOfMemory();
		return(NULL);
	}
	if ( nti_info->magic != 42 ) {
		SDL_SetError("[NSP] Invalid NTI image");
		nti_free_info(nti_info);
		return(NULL);
	}
	NSP_DPRINT("Loading NTI v%d (%dx%d)", nti_info->version, nti_info->width, nti_info->height);
	tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, nti_info->width, nti_info->height,
				   16, NSP_RMASK16, NSP_GMASK16, NSP_BMASK16, 0);
	if ( tmp == NULL ) {
		SDL_OutOfMemory();
		nti_free_info(nti_info);
		return(NULL);
	}
	data = (Uint16 *)(data + 4);
	SDL_LockSurface(tmp);
	for ( i = 0; i < nti_info->height; ++i )
		for( j = 0; j < nti_info->width; ++j)
			*(Uint16 *)NSP_SURF_PIXEL(tmp, j, i) = data[j + (nti_info->width * i)];
	SDL_UnlockSurface(tmp);
	image = SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);
	nti_free_info(nti_info);
	return(image);
}