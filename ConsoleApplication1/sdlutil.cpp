#include "stdafx.h"
#include "sdlutil.h"

SDL_SurfacePtr convert_to_opengl_format(SDL_Surface *s)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	static const Uint32 r = 0xFF000000;
	static const Uint32 g = 0x00FF0000;
	static const Uint32 b = 0x0000FF00;
	static const Uint32 a = 0x000000FF;
#else
	static const Uint32 r = 0x000000FF;
	static const Uint32 g = 0x0000FF00;
	static const Uint32 b = 0x00FF0000;
	static const Uint32 a = 0xFF000000;
#endif

	//if (SDL_SetSurfaceBlendMode(s, SDL_BLENDMODE_NONE) { throw "failed to set alpha"; }

	int w = s->w;
	int h = s->h;

	auto converted = SDL_SurfacePtr(SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, r, g, b, a));

	if (SDL_BlitSurface(s, NULL, converted.get(), NULL) != 0) { throw "failed to blit"; }

	return converted;
}
