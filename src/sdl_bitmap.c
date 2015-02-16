/*
 * sdl_bitmap.c
 *
 *  Created on: 14/12/2014
 *      Author: chronno
 */
#include <stdlib.h>     /* malloc, free, rand */
#include <mruby.h>
#include <mruby/data.h>
#include <SDL2/SDL_endian.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include "mrgsl.h"
#include "tools.h"

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
  glEnable (GL_BLEND);
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

Uint32
sdl_bitmap_getpixel (SDL_Surface *surface, int x, int y)
{
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to retrieve */
  Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

  switch (bpp)
    {
    case 1:
      return *p;
      break;

    case 2:
      return *(Uint16 *) p;
      break;

    case 3:
      if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	return p[0] << 16 | p[1] << 8 | p[2];
      else
	return p[0] | p[1] << 8 | p[2] << 16;
      break;

    case 4:
      return *(Uint32 *) p;
      break;

    default:
      return 0; /* shouldn't happen, but avoids warnings */
    }
}

void
sdl_bitmap_setpixel (SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to set */
  Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

  switch (bpp)
    {
    case 1:
      *p = pixel;
      break;

    case 2:
      *(Uint16 *) p = pixel;
      break;

    case 3:
      if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
	  p[0] = (pixel >> 16) & 0xff;
	  p[1] = (pixel >> 8) & 0xff;
	  p[2] = pixel & 0xff;
	}
      else
	{
	  p[0] = pixel & 0xff;
	  p[1] = (pixel >> 8) & 0xff;
	  p[2] = (pixel >> 16) & 0xff;
	}
      break;

    case 4:
      *(Uint32 *) p = pixel;
      break;
    }
}

void
sdl_bitmap_fillrect (SDL_Surface *surface, int x, int y, int width, int height, Uint32 pixel)
{
  SDL_Rect srcrect;
  srcrect.x = x;
  srcrect.y = y;
  srcrect.w = width;
  srcrect.h = height;
  SDL_FillRect (surface, &srcrect, pixel);
}

void
sdl_bitmap_blit (mrb_state* mrb, mrb_value src_bitmap, mrb_value src_rect, mrb_value dest_bitmap, mrb_value dest_rect)
{
  SDL_Rect src_r;
  SDL_Rect src_d;
  mrgsl_bitmap* src_bmp = DATA_PTR (src_bitmap);
  mrgsl_bitmap* des_bmp = DATA_PTR (dest_bitmap);

  src_r.x = mrb_int(mrb, mrb_get_iv (mrb, src_rect, "@x"));
  src_r.y = mrb_int(mrb, mrb_get_iv (mrb, src_rect, "@y"));
  src_r.w = mrb_int(mrb, mrb_get_iv (mrb, src_rect, "@width"));
  src_r.h = mrb_int(mrb, mrb_get_iv (mrb, src_rect, "@height"));
  src_d.x = mrb_int(mrb, mrb_get_iv (mrb, dest_rect, "@x"));
  src_d.y = mrb_int(mrb, mrb_get_iv (mrb, dest_rect, "@y"));
  src_d.w = mrb_int(mrb, mrb_get_iv (mrb, dest_rect, "@width"));
  src_d.h = mrb_int(mrb, mrb_get_iv (mrb, dest_rect, "@height"));
  SDL_BlitSurface(src_bmp->surface, &src_r, des_bmp->surface, &src_d);
}
