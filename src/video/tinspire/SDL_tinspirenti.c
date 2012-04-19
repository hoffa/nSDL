#include "SDL_video.h"
#include "SDL_stdinc.h"
#include "SDL_tinspirenti.h"

static nti_info_t *nti_get_info(Uint16 *data) {
	nti_info_t *nti_info = SDL_malloc(sizeof *nti_info);
	if(nti_info == NULL) {
		SDL_OutOfMemory();
		return NULL;
	}
	nti_info->magic = data[0] >> 8;
	nti_info->version = data[0] & 0xf;
	nti_info->width = data[1];
	nti_info->height = data[2];
	nti_info->reserved = data[3];
	return nti_info;
}

static void nti_free_info(nti_info_t *nti_info) {
	SDL_free(nti_info);
}

SDL_Surface *SDL_nLoadImage(Uint16 *data) {
	SDL_Surface *image;
	nti_info_t *nti_info = nti_get_info(data);
	int i;
	int length;
	if(nti_info == NULL) {
		SDL_OutOfMemory();
		return NULL;
	}
	if(nti_info->magic != 42) {
		SDL_SetError("[NSP] Invalid NTI image");
		nti_free_info(nti_info);
		return NULL;
	}
	NSP_DPRINT("Loading NTI v%d (%dx%d)\n",
		   nti_info->version, nti_info->width, nti_info->height);
	image = SDL_CreateRGBSurface(SDL_SWSURFACE, nti_info->width, nti_info->height,
				     NSP_BPP, NSP_RMASK, NSP_GMASK, NSP_BMASK, 0);
	if(image == NULL) {
		SDL_OutOfMemory();
		nti_free_info(nti_info);
		return NULL;
	}
#if NSP_BPP_SW8
	if ( ! SDL_nCreatePalette(image) ) {
		SDL_FreeSurface(image);
		nti_free_info(nti_info);
		return(NULL);
	}
#endif
	data = (Uint16 *)(data + 4);
	length = nti_info->width * nti_info->height;
	SDL_LockSurface(image);
	for(i = 0; i < length; ++i) {
#if NSP_BPP_SW16
		*(Uint16 *)(image->pixels + (2 * i)) = data[i];
#else
		Uint8 r, g, b;
		r = 8 * (data[i] >> 11);
		g = 4 * ((data[i] >> 5) & 0x3F);
		b = 8 * (data[i] & 0x1F);
		*(Uint8 *)(image->pixels + i) = SDL_MapRGB(image->format, r, g, b);
#endif
	}
	SDL_UnlockSurface(image);
	nti_free_info(nti_info);
	return image;
}