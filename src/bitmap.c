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

static void
bitmap_free (mrb_state *mrb, void *p)
{
  if (p)
    {
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
  count = mrb_get_args (mrb, "o|i", &first, &height);
  switch (count)
    {
    case 1:
      if (mrb_string_p(first))
	{
	  const char *str = mrb_string_value_ptr (mrb, first);
	  bitmap->surface = load_surface (str);
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
  mrb_set_iv (mrb, self, "@width", mrb_fixnum_value(bitmap->surface->w));
  mrb_set_iv (mrb, self, "@height", mrb_fixnum_value(bitmap->surface->h));
  DATA_TYPE (self) = &mrbal_bitmap_data_type;
  DATA_PTR (self) = bitmap;
  return self;
}

void
mruby_mrgsl_bitmap_init (mrb_state *mrb)
{
  struct RClass *type = mrb_define_class_under(mrb,mruby_get_mrgsl(mrb)  ,"Bitmap", mrb->object_class);
  MRB_SET_INSTANCE_TT(type, MRB_TT_DATA);
  mrb_define_method (mrb, type, "initialize", (mrb_func_t) initialize, MRB_ARGS_REQ (1) | MRB_ARGS_OPT(1));
  mrb_attr_reader (mrb, type, "width");
  mrb_attr_reader (mrb, type, "height");
}
