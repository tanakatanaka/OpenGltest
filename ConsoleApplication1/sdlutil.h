#pragma once
#include <memory>
#include "SDL_surface.h"

template <typename T>
struct CustomDeleter : std::default_delete<T>
{
};

template <>
struct CustomDeleter<SDL_Surface>
{
	void operator()(SDL_Surface *s)
	{
		SDL_FreeSurface(s);
	}
};

template <typename T>
using Ptr = std::unique_ptr<T, CustomDeleter<T>>;

using SDL_SurfacePtr = Ptr<SDL_Surface>;

SDL_SurfacePtr convert_to_opengl_format(SDL_Surface *s);
