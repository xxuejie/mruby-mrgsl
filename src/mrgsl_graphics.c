/*
 * graphics.c
 *
 *  Created on: 5/1/2015
 *      Author: chronno
 */
#include "mrgsl.h"
#include "tools.h"
#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/value.h>

static struct RClass* graphics;

static mrb_value
show (mrb_state* mrb, mrb_value self)
{
  mrb_int screenWidth;
  mrb_int screenHeight;
  mrb_value screenName;
  mrb_value rect;
  int result = 0;
  int count = mrb_get_args (mrb, "ii|o", &screenWidth, &screenHeight, &screenName);

  switch (count)
    {
    case 2:
      result = create_window (screenWidth, screenHeight, "MRGSL Game");
      break;
    case 3:
      result = create_window (screenWidth, screenHeight, mrb_string_value_ptr (mrb, screenName));
      break;
    default:
      result = create_window (640, 480, "MRGSL Game");
      break;
    }
  if (result > 0)
    {
      mrb_raise (mrb, E_ARGUMENT_ERROR, SDL_GetError ());
    }
  rect = mrb_new_instance (mrb, "Rect", 4, mrb_fixnum_value (0), mrb_fixnum_value (0), mrb_fixnum_value (screenWidth), mrb_fixnum_value (screenHeight));
  mrb_set_iv (mrb, mrb_obj_value (graphics), "@viewport", mrb_new_instance (mrb, "Viewport", 1, rect));
  mrb_set_gv(mrb, "$running", mrb_true_value());
  return mrb_nil_value ();
}

static mrb_value
update (mrb_state *mrb, mrb_value self)
{
  update_window (mrb);
  return mrb_nil_value ();
}

mrb_value
get_viewport (mrb_state* mrb, mrb_value self)
{
  return get_graphics_viewport (mrb);
}

void
mruby_mrgsl_graphics_init (mrb_state *mrb)
{
  //struct RClass *type = mrb_define_module_under (mrb, mrgsl, "Graphics");
  graphics = mrb_define_module_under (mrb, mruby_get_mrgsl (mrb), "Graphics");
  mrb_define_class_method (mrb, graphics, "show", (mrb_func_t) show, MRB_ARGS_REQ(2) | MRB_ARGS_OPT(1));
  mrb_define_class_method (mrb, graphics, "update", (mrb_func_t) update, MRB_ARGS_NONE());
  mrb_define_class_method (mrb, graphics, "viewport", (mrb_func_t)get_viewport, MRB_ARGS_NONE ());
}
