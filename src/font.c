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
  mrb_int size;
  mrb_int count;
  mrgsl_font* font = (mrgsl_font*) malloc (sizeof(mrgsl_font));
  count = mrb_get_args (mrb, "o|i", &fontname);
  if (count == 1 && mrb_string_p(fontname))
    {
      name = mrb_string_value_ptr (mrb, fontname);
      font->font = TTF_OpenFont (name, 12);
      if (!font->font)
	{
	  printf ("TTF_OpenFont: %s\n", TTF_GetError ());
	  mrb_raise (mrb, E_ARGUMENT_ERROR, "Asset not found");
	  return mrb_nil_value ();
	}
      mrb_set_iv (mrb, self, "@name", fontname);
      mrb_set_iv (mrb, self, "@size", mrb_fixnum_value (12));
    }
  else if (count == 2 && &&mrb_string_p(fontname))
    {
      name = mrb_string_value_ptr (mrb, fontname);
      font->font = TTF_OpenFont (name, 12);
      if (!font->font)
	{
	  mrb_raise (mrb, E_ARGUMENT_ERROR, "Asset not found");
	  return mrb_nil_value ();
	}
      mrb_set_iv (mrb, self, "@name", fontname);
      mrb_set_iv (mrb, self, "@size", mrb_fixnum_value (size));
    }
  else
    {
      mrb_raise (mrb, E_ARGUMENT_ERROR, "Wrong type of arguments");
      return mrb_nil_value ();
    }
  mrb_value zero = mrb_fixnum_value(0);
  mrb_value full = mrb_fixnum_value(255);
  mrb_set_iv (mrb, self, "@color", mrb_new_instance(mrb, "Color", 4, zero, zero, zero, full));
  mrb_set_iv (mrb, self, "@bold", mrb_false_value());
  mrb_set_iv (mrb, self, "@italic", mrb_false_value());
  mrb_set_iv (mrb, self, "@underline", mrb_false_value());
  DATA_TYPE (self) = &mrbal_font_data_type;
  DATA_PTR (self) = font;
  return self;
}

void
mruby_mrgsl_font_init (mrb_state *mrb)
{
  struct RClass *type = mrb_define_class_under (mrb, mruby_get_mrgsl (mrb), "Font", mrb->object_class);
  MRB_SET_INSTANCE_TT(type, MRB_TT_DATA);
  mrb_define_method (mrb, type, "initialize", (mrb_func_t) initialize, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  mrb_attr_reader (mrb, type, "name");
  mrb_attr_reader (mrb, type, "size");
  mrb_attr_accessor (mrb, type, "color");
  mrb_attr_accessor (mrb, type, "bold");
  mrb_attr_accessor (mrb, type, "italic");
  mrb_attr_accessor (mrb, type, "underline");
}
