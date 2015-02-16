/*
 * viewport.c
 *
 *  Created on: 3/1/2015
 *      Author: chronno
 */
#include "mrgsl.h"
#include "tools.h"
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/array.h>
#include <mruby/data.h>
#include <stdlib.h>

void
viewport_free (mrb_state *mrb, void *p)
{
  if (p)
    {
      free (p);
    }
}

struct mrb_data_type const mrbal_viewport_data_type =
  { "mrgsl/Viewport", viewport_free };

static mrb_value
initialize (mrb_state *mrb, mrb_value self)
{
  mrb_value rect;
  mrb_value parent;
  int count = mrb_get_args (mrb, "o|o", &rect, &parent);
  if (count == 1)
    {
      mrb_set_iv (mrb, self, "@rect", rect);
      mrb_set_iv (mrb, self, "@parent", mrb_nil_value ());
      mrb_set_iv (mrb, self, "@visible", mrb_true_value ());
      mrb_set_iv (mrb, self, "@z", mrb_fixnum_value (0));
      mrb_set_iv (mrb, self, "children", mrb_ary_new(mrb));
      mrb_set_iv (mrb, self, "sorted?", mrb_true_value ());
      if (!mrb_is_equals (mrb, get_graphics_viewport(mrb), mrb_nil_value ()))
	{
	  mrgsl_viewport_add_child (mrb, get_graphics_viewport(mrb), self);
	}
    }
  else if (count == 2 && mrb_is_a (mrb, parent, "Viewport"))
    {
      mrb_set_iv (mrb, self, "@rect", rect);
      mrb_set_iv (mrb, self, "@parent", parent);
      mrb_set_iv (mrb, self, "@visible", mrb_true_value ());
      mrb_set_iv (mrb, self, "@z", mrb_fixnum_value (0));
      mrb_set_iv (mrb, self, "children", mrb_ary_new(mrb));
      mrb_set_iv (mrb, self, "sorted?", mrb_true_value ());
      mrgsl_viewport_add_child (mrb, parent, self);
    }
  else
    {
      mrb_raise (mrb, E_ARGUMENT_ERROR, "Wrong number of arguments");
    }
  return self;
}

void
mruby_mrgsl_viewport_init (mrb_state *mrb)
{
  struct RClass *type = mrb_define_class_under (mrb, mruby_get_mrgsl(mrb), "Viewport", mrb->object_class);
  mrb_define_method (mrb, type, "initialize", (mrb_func_t) initialize, MRB_ARGS_OPT(2));
  mrb_attr_accessor (mrb, type, "rect");
  mrb_attr_accessor (mrb, type, "visible");
  mrb_attr_accessor (mrb, type, "parent");
  mrb_attr_accessor (mrb, type, "z");
}
