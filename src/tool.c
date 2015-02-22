/*
 * tools.c
 *
 *  Created on: 30/12/2014
 *      Author: chronno
 */
#include "tools.h"
#include <stdlib.h>
#include <string.h>
#include "mrgsl.h"
#include <mruby.h>
#include <mruby/value.h>
#include <mruby/variable.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include <mruby/array.h>
#include "sort.h"

#ifndef M_PI
  #define M_PI		3.14159265358979323846
#endif

void
mrb_attr_reader (mrb_state* mrb, struct RClass* type, const char* varname)
{
  mrb_value classobj;
  mrb_value sym;
  classobj = mrb_obj_value (type);
  sym = mrb_str_new_cstr (mrb, varname);
  mrb_funcall (mrb, classobj, "attr_reader", 1, sym);
}

void
mrb_attr_writer (mrb_state* mrb, struct RClass* type, const char* varname)
{
  mrb_value classobj;
  mrb_value sym;
  classobj = mrb_obj_value (type);
  sym = mrb_str_new_cstr (mrb, varname);
  mrb_funcall (mrb, classobj, "attr_writer", 1, sym);
}

void
mrb_attr_accessor (mrb_state* mrb, struct RClass* type, const char* varname)
{
  mrb_value classobj;
  mrb_value sym;
  classobj = mrb_obj_value (type);
  sym = mrb_str_new_cstr (mrb, varname);
  mrb_funcall (mrb, classobj, "attr_accessor", 1, sym);
}

void
mrb_set_gv (mrb_state* mrb, const char *name, mrb_value val)
{
  mrb_gv_set (mrb, mrb_intern_static (mrb, name, strlen (name)), val);
}

mrb_value
mrb_get_gv (mrb_state* mrb, const char *name)
{
  return mrb_gv_get (mrb, mrb_intern_static (mrb, name, strlen (name)));
}

void
mrb_set_iv (mrb_state* mrb, mrb_value object, const char *name, mrb_value val)
{
  mrb_iv_set (mrb, object, mrb_intern_static (mrb, name, strlen (name)), val);
}

mrb_value
mrb_get_iv (mrb_state* mrb, mrb_value object, const char *name)
{
  return mrb_iv_get (mrb, object, mrb_intern_static (mrb, name, strlen (name)));
}

mrb_bool
mrb_is_equals (mrb_state* mrb, mrb_value object, mrb_value other)
{
  return mrb_equal (mrb, object, other);
}

mrb_bool
mrb_is_a (mrb_state* mrb, mrb_value object, const char* classname)
{
  struct RClass* mrgsl = mrb_module_get (mrb, "MRGSL");
  struct RClass* class = mrb_class_get_under (mrb, mrgsl, classname);
  return mrb_obj_is_instance_of (mrb, object, class);
}

mrb_value
mrb_new_instance (mrb_state* mrb, const char* class, mrb_int argc, ...)
{
  va_list arguments;
  mrb_value* params;
  int i;
  struct RClass *mrgsl;
  struct RClass *type;
  if (argc > 0)
    {
      params = malloc (sizeof(mrb_value) * argc);

      va_start(arguments, argc);
      for (i = 0; i < argc; i++)
	{
	  params[i] = va_arg(arguments, mrb_value);
	}
      va_end(arguments);
    }
  else
    {
      params = NULL;
    }
  mrgsl = mrb_module_get (mrb, "MRGSL");
  type = mrb_class_get_under (mrb, mrgsl, class);
  return mrb_obj_new (mrb, type, argc, params);
}

static
int
zcompare (void* param, const void * a, const void * b)
{
  mrb_value f;
  mrb_value s;
  mrb_int fz;
  mrb_int sz;
  mrb_state* mrb = (mrb_state*) param;
  f = *(mrb_value*) a;
  s = *(mrb_value*) b;
  fz = mrb_int(mrb, mrb_get_iv (mrb, f, "@z"));
  sz = mrb_int(mrb, mrb_get_iv (mrb, s, "@z"));

  if (fz > sz)
    {
      return 1;
    }
  else if (fz < sz)
    {
      return -1;
    }
  else
    {
      return 0;
    }
}

void
mrgsl_sort_viewport_children (mrb_state* mrb, mrb_value arry)
{
  struct RArray* anArray = mrb_ary_ptr(arry);
  qsort_r (anArray->ptr, anArray->len, sizeof(mrb_value), mrb, zcompare);
}

void
mrgsl_viewport_add_child (mrb_state* mrb, mrb_value parent, mrb_value child)
{
  mrb_value arry = mrb_get_iv (mrb, parent, "children");
  mrb_ary_push (mrb, arry, child);
  mrb_set_iv (mrb, parent, "sorted?", mrb_false_value ());
}

void
mrgsl_viewport_remove_child (mrb_state* mrb, mrb_value parent, mrb_value child)
{
  mrgsl_viewport* viewport = (mrgsl_viewport*) DATA_PTR(parent);
  for (int i = viewport->size - 1; i >= 0; --i)
    {
      if (mrb_is_equals (mrb, viewport->children[i], child))
	{
	  viewport->size -= 1;
	  viewport->children[i] = mrb_nil_value ();
	  for (int t = i; t < viewport->size; t++)
	    {
	      viewport->children[t] = viewport->children[t + 1];
	    }
	}
    }
}

void
mrgsl_draw_viewport (mrb_state* mrb, mrb_value viewport)
{
  mrb_value arry = mrb_get_iv (mrb, viewport, "children");
  mrb_value sorted = mrb_get_iv (mrb, viewport, "sorted?");
  if (mrb_is_equals (mrb, sorted, mrb_false_value ()))
    {
      mrgsl_sort_viewport_children (mrb, arry);
    }
  for (int i = 0; i < mrb_ary_len (mrb, arry); i++)
    {
      mrb_value child = mrb_ary_entry (arry, i);
      mrb_value visible = mrb_get_iv (mrb, child, "@visible");
      if (mrb_is_equals (mrb, visible, mrb_true_value ()))
	{
	  if (mrb_is_a (mrb, child, "Viewport"))
	    {
	      mrgsl_draw_viewport (mrb, child);
	    }
	  else if (mrb_is_a (mrb, child, "Sprite"))
	    {
	      mrgsl_draw_sprite (mrb, child);
	    }
	}
    }
}

mrb_value
get_graphics_viewport (mrb_state* mrb)
{
  mrb_value graphics = mrb_obj_value (mrb_module_get_under (mrb, mruby_get_mrgsl (mrb), "Graphics"));
  return mrb_get_iv (mrb, graphics, "@viewport");
}

static void
get_rect_values (mrb_state* mrb, mrb_value rect, mrb_int* rx, mrb_int* ry, mrb_int* rw, mrb_int* rh)
{
  *rx = mrb_int(mrb, mrb_get_iv (mrb, rect, "@x"));
  *ry = mrb_int(mrb, mrb_get_iv (mrb, rect, "@y"));
  *rw = mrb_int(mrb, mrb_get_iv (mrb, rect, "@width"));
  *rh = mrb_int(mrb, mrb_get_iv (mrb, rect, "@height"));
}

void
draw_quads (mrb_int spr_x, mrb_int view_x, mrb_int spr_y, mrb_int view_y, mrb_int spr_ox, mrb_int spr_oy, mrb_float spr_angle, mrb_float spr_zx, mrb_float spr_zy ,float rx, float ry, float rw, float rh,  mrb_int rect_w, mrb_int rect_h, mrgsl_bitmap* bitmap)
{
  glBindTexture ( GL_TEXTURE_2D, bitmap->texture);
  glLoadIdentity ();
  glTranslatef (spr_x + view_x + spr_ox, spr_y + view_y + spr_oy, 0);
  glRotatef( -spr_angle , 0.0f, 0.0f, 1.0f);
  glTranslatef(-(spr_ox * spr_zx), -(spr_oy * spr_zy), 0);


  glEnable (GL_TEXTURE_2D);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBegin ( GL_QUADS);
  //top-left vertex (corner)
  glTexCoord2i (rx, ry);
  glVertex3f (spr_x, spr_y, 0.f);
  //top-right vertex (corner)
  glTexCoord2f (rx + rw, ry);
  glVertex3f ((spr_x + rect_w) * spr_zx, spr_y, 0.f);
  //bottom-right vertex (corner)
  glTexCoord2f (rx + rw, ry + rh);
  glVertex3f ((spr_x + rect_w) * spr_zx, (spr_y + rect_h) * spr_zy, 0.f);
  //bottom-left vertex (corner)
  glTexCoord2f (rx, ry + rh);
  glVertex3f (spr_x, (spr_y + rect_h) * spr_zy, 0.f);
  glEnd ();
}

void
check_rect (mrb_state* mrb, mrb_value sprite, mrgsl_bitmap* bitmap)
{
  mrb_value rect;
  rect = mrb_get_iv (mrb, sprite, "@rect");
  if (mrb_is_equals (mrb, rect, mrb_nil_value ()))
    {
      mrb_value nrect = mrb_new_instance (mrb, "Rect", 4, 0, 0, bitmap->surface->w, bitmap->surface->h);
      rect = nrect;
      mrb_set_iv (mrb, sprite, "@rect", nrect);
    }
}

static mrb_bool
is_bitmap_valid (mrb_state* mrb, mrb_value sprite, mrgsl_bitmap** bitmap)
{
  mrb_value mrb_bitmap = mrb_get_iv (mrb, sprite, "@bitmap");
  if (mrb_is_equals (mrb, mrb_bitmap, mrb_nil_value ()))
    {
      return FALSE;
    }
  *bitmap = (mrgsl_bitmap*) DATA_PTR(mrb_bitmap);
  if (bitmap == NULL)
    {
      return FALSE;
    }
  return TRUE;
}

void
mrgsl_draw_sprite (mrb_state* mrb, mrb_value sprite)
{
  mrb_value rect;
  mrb_int rect_x, rect_y, rect_w, rect_h;
  mrb_int spr_x, spr_y, spr_ox, spr_oy;
  mrb_float spr_angle, spr_zx, spr_zy;
  mrb_value parent, prect;
  mrb_int view_x, view_y;
  float rx, ry, rw, rh;
  mrgsl_bitmap* bitmap;
  if (!is_bitmap_valid (mrb, sprite, &bitmap))
    {
      return;
    }
  rect = mrb_get_iv (mrb, sprite, "@rect");
  check_rect (mrb, sprite, bitmap);
  get_rect_values (mrb, rect, &rect_x, &rect_y, &rect_w, &rect_h);
  spr_x = mrb_int(mrb, mrb_get_iv (mrb, sprite, "@x"));
  spr_y = mrb_int(mrb, mrb_get_iv (mrb, sprite, "@y"));
  spr_angle = mrb_to_flo(mrb, mrb_get_iv (mrb, sprite, "@angle"));
  spr_zx = mrb_to_flo(mrb, mrb_get_iv (mrb, sprite, "@zoom_x"));
  spr_zy = mrb_to_flo(mrb, mrb_get_iv (mrb, sprite, "@zoom_y"));
  spr_ox = mrb_int(mrb, mrb_get_iv (mrb, sprite, "@ox"));
  spr_oy = mrb_int(mrb, mrb_get_iv (mrb, sprite, "@oy"));
  parent = mrb_get_iv (mrb, sprite, "@parent");
  prect = mrb_get_iv (mrb, parent, "@rect");
  view_x = mrb_int(mrb, mrb_get_iv (mrb, prect, "@x"));
  view_y = mrb_int(mrb, mrb_get_iv (mrb, prect, "@y"));
  rx = rect_x / (float) bitmap->surface->w;
  ry = rect_y / (float) bitmap->surface->h;
  rw = rect_w / (float) bitmap->surface->w;
  rh = rect_h / (float) bitmap->surface->h;
  draw_quads (spr_x, view_x, spr_y, view_y, spr_ox, spr_oy, spr_angle, spr_zx, spr_zy,  rx, ry, rw, rh, rect_w, rect_h, bitmap);
}

