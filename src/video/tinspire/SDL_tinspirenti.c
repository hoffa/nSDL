#include "SDL_video.h"
#include "SDL_stdinc.h"
#include "SDL_tinspirenti.h"

static nti_info_t *nti_get_info(Uint16 *data)
{
	nti_info_t *nti_info = SDL_malloc(sizeof *nti_info);
	if(nti_info == NULL) {
		SDL_OutOfMemory();
		return(NULL);
	}
	nti_info->magic = data[0] >> 8;
	nti_info->version = data[0] & 0xf;
	nti_info->width = data[1];
	nti_info->height = data[2];
	nti_info->reserved = data[3];
	return(nti_info);
}

static void nti_free_info(nti_info_t *nti_info)
{
	SDL_free(nti_info);
}

SDL_Surface *SDL_nLoadImage(Uint16 *data)
{
	SDL_Surface *tmp, *image;
	nti_info_t *nti_info = nti_get_info(data);
	int i;
	int length;
	if ( nti_info == NULL ) {
		SDL_OutOfMemory();
		return(NULL);
	}
	if ( nti_info->magic != 42 ) {
		SDL_SetError("[NSP] Invalid NTI image");
		nti_free_info(nti_info);
		return(NULL);
	}
	NSP_DPRINT("Loading NTI v%d (%dx%d)\n",
		   nti_info->version, nti_info->width, nti_info->height);
	tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, nti_info->width, nti_info->height,
				   16, 0xF800, 0x7E0, 0x1F, 0);
	if ( tmp == NULL ) {
		SDL_OutOfMemory();
		nti_free_info(nti_info);
		return(NULL);
	}
	data = (Uint16 *)(data + 4);
	length = nti_info->width * nti_info->height;
	SDL_LockSurface(tmp);
	for ( i = 0; i < length; ++i )
		*(Uint16 *)(tmp->pixels + (2 * i)) = data[i];
	SDL_UnlockSurface(tmp);
	if(NSP_BPP == 8) {
		image = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
	} else
		image = tmp;
	nti_free_info(nti_info);
	return(image);
}