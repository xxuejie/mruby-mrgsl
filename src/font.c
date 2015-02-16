/*
 * font.c
 *
 *  Created on: 15/2/2015
 *      Author: Manuel
 */
#include "mrgsl.h"
#include "tools.h"
#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>

static void
font_free (mrb_state *mrb, void *p)
{
  if (p)
    {
      mrgsl_font* font = (mrgsl_font*) p;
      SDL_free (font->font);
      free (p);
    }
}
struct mrb_data_type const mrbal_font_data_type =
  { "mrgsl/Font", font_free };

static mrb_value
initialize (mrb_state* mrb, mrb_value self)
{
  const char* name;
  mrb_value fontname;
  mrgsl_font* font = (mrgsl_font*) malloc (sizeof(mrgsl_font));
  mrb_get_args (mrb, "o", &fontname);
  if (mrb_string_p(fontname))
    {
      name = mrb_string_value_ptr (mrb, fontname);
      font->font = TTF_OpenFont (name, 12);
      if (!font->font)
	{
	  printf("TTF_OpenFont: %s\n", TTF_GetError());
	  mrb_raise (mrb, E_ARGUMENT_ERROR, "Asset not found");
	  return mrb_nil_value();
	}
      mrb_set_iv (mrb, self, "@name", fontname);
    }
  else
    {
      mrb_raise (mrb, E_ARGUMENT_ERROR, "Wrong type of arguments");
    }
  DATA_TYPE (self) = &mrbal_font_data_type;
  DATA_PTR (self) = font;
  return self;
}

void
mruby_mrgsl_font_init (mrb_state *mrb)
{
  struct RClass *type = mrb_define_class_under (mrb, mruby_get_mrgsl (mrb), "Font", mrb->object_class);
  MRB_SET_INSTANCE_TT(type, MRB_TT_DATA);
  mrb_define_method (mrb, type, "initialize", (mrb_func_t) initialize, MRB_ARGS_REQ(1));
  mrb_attr_reader (mrb, type, "name");
}
