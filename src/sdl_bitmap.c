/*
 * sdl_bitmap.c
 *
 *  Created on: 14/12/2014
 *      Author: chronno
 */
#include <stdlib.h>     /* malloc, free, rand */
#include <mruby.h>
#include <SDL2/SDL_endian.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include "mrgsl.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
int rmask = 0xff000000;
int gmask = 0x00ff0000;
int bmask = 0x0000ff00;
int amask = 0x000000ff;
#else
int rmask = 0x000000ff;
int gmask = 0x0000ff00;
int bmask = 0x00ff0000;
int amask = 0xff000000;
#endif

GLuint
surface_texture (SDL_Surface *surface)
{
  GLuint tid;
  GLenum texture_format;
  GLint ncolors;
  SDL_Surface* s = surface;

  /* Convert SDL_Surface to OpenGL Texture */
  ncolors = s->format->BytesPerPixel;
  if (ncolors == 4)
    {
      //alpha channel
      if (s->format->Rmask == 0x000000ff)
	{
	  texture_format = GL_RGBA;
	}
      else
	{
	  texture_format = GL_BGRA;
	}
    }
  else
    {
      if (ncolors == 3)
	{
	  //no alpha channel
	  if (s->format->Rmask == 0x000000ff)
	    texture_format = GL_RGB;
	  else
	    texture_format = GL_BGR;
	}
      else
	{
	  return 0;
	}
    }
  glGenTextures (1, &tid);
  glEnable (GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBindTexture (GL_TEXTURE_2D, tid);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D (GL_TEXTURE_2D, 0, ncolors, s->w, s->h, 0, texture_format,
  GL_UNSIGNED_BYTE,
		s->pixels);

  return tid;
}

mrb_int
surface_width (SDL_Surface *surface)
{
  return (mrb_int) surface->w;
}
mrb_int
surface_height (SDL_Surface *surface)
{
  return (mrb_int) surface->h;
}

SDL_Surface *
create_surface (mrb_int width, mrb_int height)
{
  return SDL_CreateRGBSurface (0, width, height, 32, rmask, gmask, bmask, amask);
}

SDL_Surface *
load_surface (const char* filename)
{
  return IMG_Load (filename);
}

