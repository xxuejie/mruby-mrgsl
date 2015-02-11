/*
 * mrgsl.c
 *
 *  Created on: 11/2/2015
 *      Author: Manuel
 */
#include <mruby.h>
#include <stdio.h>
#include "mrgsl.h"

void
mruby_mrgsl_init (mrb_state *mrb){
  mrb_define_module(mrb, "MRGSL");
}

struct RClass*
mruby_get_mrgsl (mrb_state *mrb){
  return mrb_module_get(mrb, "MRGSL");
}
