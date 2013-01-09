#include <os.h>
#include "SDL_video.h"
#include "SDL_tinspirevideo.h"

int nSDL_EnableRelativePaths(char **argv)
{
	char buf[NSP_BUF_SIZE], *p;
	strcpy(buf, argv[0]);
	p = strrchr(buf, '/');
	if ( ! p )
		return(-1);
	*p = '\0';
	return(NU_Set_Current_Dir(buf) ? -1 : 0);
}