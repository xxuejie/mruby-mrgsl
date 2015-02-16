/*
 * tools.h
 *
 *  Created on: 30/12/2014
 *      Author: chronno
 */

#ifndef MRUBY_TOOLS_H_
#define MRUBY_TOOLS_H_
#include <mruby.h>
#include "mrgsl.h"

typedef int cmp_t(void *, const void *, const void *);

void
mrb_attr_reader (mrb_state* mrb, struct RClass* type, const char* varname);

void
mrb_attr_writer (mrb_state* mrb, struct RClass* type, const char* varname);

void
mrb_attr_accessor (mrb_state* mrb, struct RClass* type, const char* varname);

void
mrb_set_gv (mrb_state* mrb, const char *name, mrb_value val);

void
mrb_set_iv (mrb_state* mrb, mrb_value object, const char *name, mrb_value val);

mrb_value
mrb_get_gv (mrb_state* mrb, const char *name);

mrb_value
mrb_get_iv (mrb_state* mrb, mrb_value object, const char *name);

mrb_bool
mrb_is_equals (mrb_state* mrb, mrb_value object, mrb_value other);

mrb_value
mrb_new_instance (mrb_state* mrb, const char* class, mrb_int argc, ...);

mrb_bool
mrb_is_a (mrb_state* mrb, mrb_value object, const char* classname);

mrb_value
get_graphics_viewport (mrb_state* mrb);

void
mrgsl_viewport_remove_child (mrb_state* mrb, mrb_value parent, mrb_value child);

void
mrgsl_viewport_add_child (mrb_state* mrb, mrb_value parent, mrb_value child);

void
mrgsl_draw_viewport (mrb_state* mrb, mrb_value viewport);

void
mrgsl_draw_sprite (mrb_state* mrb, mrb_value sprite);

Uint32
sdl_bitmap_getpixel (SDL_Surface *surface, int x, int y);

void
sdl_bitmap_setpixel (SDL_Surface *surface, int x, int y, Uint32 pixel);

void
sdl_bitmap_fillrect (SDL_Surface *surface, int x, int y, int width, int height, Uint32 pixel);

void
sdl_bitmap_blit (mrb_state* mrb, mrb_value src_bitmap, mrb_value src_rect, mrb_value dest_bitmap, mrb_value dest_rect);

void qsort_r(void *a, size_t n, size_t es, void *thunk, cmp_t *cmp);

void
mrgsl_sort_viewport_children (mrb_state* mrb,mrb_value arry);
#endif /* MRUBY_TOOLS_H_ */

