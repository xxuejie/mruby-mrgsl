#include <mruby.h>
#include <stdio.h>
#include "mrgsl.h"

void
mrb_mrgsl_gem_init (mrb_state* mrb)
{

  mruby_mrgsl_bitmap_init (mrb);
  mruby_mrgsl_viewport_init (mrb);
  mruby_mrgsl_sprite_init (mrb);
  mruby_mrgsl_graphics_init (mrb);
  mruby_mrgsl_keyboard_init(mrb);
}

void
mrb_mrgsl_gem_final (mrb_state* mrb)
{
  /* finalizer */
}
