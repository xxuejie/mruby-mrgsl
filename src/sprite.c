/*
 * sprite.c
 *
 *  Created on: 3/1/2015
 *      Author: chronno
 */
#include "mrgsl.h"
#include "tools.h"
#include <mruby.h>
#include <mruby/class.h>

static mrb_value
initialize (mrb_state *mrb, mrb_value self)
{
  mrb_value parent;
  mrb_int count = mrb_get_args (mrb, "|o", &parent);
  mrb_set_iv (mrb, self, "@rect", mrb_new_instance (mrb, "Rect", 0, NULL));
  mrb_set_iv (mrb, self, "@bitmap", mrb_nil_value ());
  mrb_set_iv (mrb, self, "@visible", mrb_true_value ());
  mrb_set_iv (mrb, self, "@x", mrb_fixnum_value (0));
  mrb_set_iv (mrb, self, "@y", mrb_fixnum_value (0));
  mrb_set_iv (mrb, self, "@z", mrb_fixnum_value (0));
  mrb_set_iv (mrb, self, "@angle", mrb_fixnum_value (0));
  mrb_set_iv (mrb, self, "@ox", mrb_fixnum_value (0));
  mrb_set_iv (mrb, self, "@oy", mrb_fixnum_value (0));
  mrb_set_iv (mrb, self, "@zoom_x", mrb_float_value (mrb, 1));
  mrb_set_iv (mrb, self, "@zoom_y", mrb_float_value (mrb, 1));
  mrb_set_iv (mrb, self, "@mirror", mrb_false_value ());
  if (count == 1)
    {
      mrb_set_iv (mrb, self, "@parent", parent);
      mrgsl_viewport_add_child (mrb, parent, self);
    }
  else
    {
      mrb_set_iv (mrb, self, "@parent", get_graphics_viewport (mrb));
      mrgsl_viewport_add_child (mrb, get_graphics_viewport (mrb), self);
    }

  return self;
}

static mrb_value
set_z(mrb_state* mrb, mrb_value self){
  mrb_int z;
  mrb_value parent;
  mrb_get_args(mrb, "i", &z);
  mrb_set_iv(mrb, self, "@z", mrb_fixnum_value(z));
  parent = mrb_get_iv(mrb, self, "@parent");
  mrb_set_iv (mrb, parent, "sorted?", mrb_false_value());
  return self;
}

void
mruby_mrgsl_sprite_init (mrb_state *mrb)
{
  struct RClass *type = mrb_define_class_under (mrb, mruby_get_mrgsl (mrb), "Sprite", mrb->object_class);
  mrb_define_method (mrb, type, "initialize", (mrb_func_t) initialize, MRB_ARGS_OPT(1));
  mrb_define_method (mrb, type, "z=", (mrb_func_t) set_z, MRB_ARGS_REQ(1));
  mrb_attr_accessor (mrb, type, "rect");
  mrb_attr_accessor (mrb, type, "visible");
  mrb_attr_accessor (mrb, type, "parent");
  mrb_attr_accessor (mrb, type, "bitmap");
  mrb_attr_accessor (mrb, type, "x");
  mrb_attr_accessor (mrb, type, "y");
  mrb_attr_reader (mrb, type, "z");
  mrb_attr_accessor (mrb, type, "angle");
  mrb_attr_accessor (mrb, type, "ox");
  mrb_attr_accessor (mrb, type, "oy");
  mrb_attr_accessor (mrb, type, "zoom_x");
  mrb_attr_accessor (mrb, type, "zoom_y");
  mrb_attr_accessor (mrb, type, "mirror");


}

