/*
 * bitmap.c
 *
 *  Created on: 3/1/2015
 *      Author: chronno
 */
#include "mrgsl.h"
#include "tools.h"
#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <SDL2/SDL_surface.h>
static void
bitmap_free (mrb_state *mrb, void *p)
{
  if (p)
    {
      mrgsl_bitmap* bitmap = (mrgsl_bitmap*)p;
      SDL_FreeSurface(bitmap->surface);
      free(bitmap->texture);
      free (p);
    }
}

struct mrb_data_type const mrbal_bitmap_data_type =
  { "mrgsl/Bitmap", bitmap_free };

static mrb_value
initialize (mrb_state *mrb, mrb_value self)
{
  mrgsl_bitmap* bitmap = (mrgsl_bitmap*) malloc (sizeof(mrgsl_bitmap));
  mrb_value height;
  mrb_value first;
  mrb_int h;
  mrb_int w;
  int count;
  count = mrb_get_args (mrb, "o|o", &first, &height);
  switch (count)
    {
    case 1:
      if (mrb_string_p(first))
	{
	  const char *str = mrb_string_value_ptr (mrb, first);
	  bitmap->surface = load_surface (str);
	  if(bitmap->surface == NULL){
	      mrb_raise (mrb, E_ARGUMENT_ERROR, "Asset not found");
	      return mrb_nil_value();
	  }
	  bitmap->texture = surface_texture (bitmap->surface);
	}
      else
	{
	  mrb_raise (mrb, E_ARGUMENT_ERROR, "Wrong type of arguments");
	}
      break;
    case 2:
      h = mrb_int(mrb, height);
      w = mrb_int(mrb, first);
      bitmap->surface = create_surface (w, h);
      bitmap->texture = surface_texture (bitmap->surface);
      break;
    default:
      mrb_raise (mrb, E_ARGUMENT_ERROR, "Wrong number of arguments");
      return mrb_nil_value ();
      break;
    }
  mrb_set_iv (mrb, self, "@width", mrb_fixnum_value (bitmap->surface->w));
  mrb_set_iv (mrb, self, "@height", mrb_fixnum_value (bitmap->surface->h));
  DATA_TYPE (self) = &mrbal_bitmap_data_type;
  DATA_PTR (self) = bitmap;
  return self;
}

static mrb_value
mrb_bitmap_get_pixel (mrb_state *mrb, mrb_value self)
{
  mrb_int x, y;
  Uint8 r, g, b, a = 0;
  mrgsl_bitmap* bitmap = DATA_PTR(self);
  SDL_Surface* surface = bitmap->surface;
  mrb_get_args (mrb, "ii", &x, &y);
  if (x < 0 || x >= surface->w || y <= 0 || y >= surface->h)
    {
      r = g = b = a = 0;
    }
  else
    {
      Uint32 pix = sdl_bitmap_getpixel (surface, x, y);

      SDL_GetRGBA (pix, surface->format, &r, &g, &b, &a);

    }
  return mrb_new_instance (mrb, "Color", 4, mrb_fixnum_value (r), mrb_fixnum_value (g), mrb_fixnum_value (b), mrb_fixnum_value (a));
}

static mrb_value
mrb_bitmap_set_pixel (mrb_state *mrb, mrb_value self)
{
  mrb_int x;
  mrb_int y;
  mrb_value color;
  mrb_int r;
  mrb_int g;
  mrb_int b;
  mrb_int a;
  Uint32 pixel;
  mrgsl_bitmap* bitmap = DATA_PTR(self);
  SDL_Surface* surface = bitmap->surface;
  mrb_get_args (mrb, "iio", &x, &y, &color);
  r = mrb_int(mrb, mrb_get_iv (mrb, color, "@r"));
  g = mrb_int(mrb, mrb_get_iv (mrb, color, "@g"));
  b = mrb_int(mrb, mrb_get_iv (mrb, color, "@b"));
  a = mrb_int(mrb, mrb_get_iv (mrb, color, "@a"));
  pixel = SDL_MapRGBA (surface->format, r, g, b, a);
  sdl_bitmap_setpixel (surface, x, y, pixel);
  bitmap->texture = surface_texture (surface);
  return color;
}

static mrb_value
mrb_bitmap_fill_rect (mrb_state* mrb, mrb_value self)
{
  mrb_value color;
  mrb_value rect;
  mrb_int x, y, width, height, r, g, b, a;
  Uint32 pixel;
  mrgsl_bitmap* bitmap = DATA_PTR(self);
  SDL_Surface* surface = bitmap->surface;
  mrb_get_args (mrb, "oo", &rect, &color);
  x = mrb_int(mrb, mrb_get_iv (mrb, rect, "@x"));
  y = mrb_int(mrb, mrb_get_iv (mrb, rect, "@y"));
  width = mrb_int(mrb, mrb_get_iv (mrb, rect, "@width"));
  height = mrb_int(mrb, mrb_get_iv (mrb, rect, "@height"));
  r = mrb_int(mrb, mrb_get_iv (mrb, color, "@r"));
  g = mrb_int(mrb, mrb_get_iv (mrb, color, "@g"));
  b = mrb_int(mrb, mrb_get_iv (mrb, color, "@b"));
  a = mrb_int(mrb, mrb_get_iv (mrb, color, "@a"));
  pixel = SDL_MapRGBA (surface->format, r, g, b, a);
  sdl_bitmap_fillrect(surface, x,y,width,height, pixel);
  bitmap->texture = surface_texture (surface);
  return self;
}

static mrb_value
mrb_bitmap_blit(mrb_state* mrb, mrb_value self){
  mrb_value src_rect;
  mrb_value src_bitm;
  mrb_value des_rect;
  mrgsl_bitmap* bitmap = DATA_PTR(self);
  mrb_get_args(mrb, "ooo", &src_bitm, &src_rect, &des_rect);
  sdl_bitmap_blit(mrb, src_bitm, src_rect, self, des_rect);
  bitmap->texture = surface_texture (bitmap->surface);
  return self;
}

void
mruby_mrgsl_bitmap_init (mrb_state *mrb)
{
  struct RClass *type = mrb_define_class_under (mrb, mruby_get_mrgsl (mrb), "Bitmap", mrb->object_class);
  MRB_SET_INSTANCE_TT(type, MRB_TT_DATA);
  mrb_define_method (mrb, type, "initialize", (mrb_func_t) initialize, MRB_ARGS_REQ (1) | MRB_ARGS_OPT(1));
  mrb_define_method (mrb, type, "set_pixel", (mrb_func_t) mrb_bitmap_set_pixel, MRB_ARGS_REQ(3));
  mrb_define_method (mrb, type, "get_pixel", (mrb_func_t) mrb_bitmap_get_pixel, MRB_ARGS_REQ(2));
  mrb_define_method (mrb, type, "fill_rect", (mrb_func_t) mrb_bitmap_fill_rect, MRB_ARGS_REQ(2));
  mrb_define_method (mrb, type, "blt", (mrb_func_t) mrb_bitmap_blit, MRB_ARGS_REQ(3));
  mrb_attr_reader (mrb, type, "width");
  mrb_attr_reader (mrb, type, "height");

}
