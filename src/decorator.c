/*
 * decorator.c
 *
 *  Created on: 16/2/2015
 *      Author: Carnage
 */

#include "mrgsl.h"
#include "tools.h"
#include <mruby.h>
#include <mruby/class.h>

static mrb_value initialize(mrb_state* mrb, mrb_value self) {
	mrb_int count;
	mrb_value size = mrb_fixnum_value(12);
	mrb_value color = mrb_new_instance(mrb, "Color", 4, mrb_fixnum_value(0),
			mrb_fixnum_value(0), mrb_fixnum_value(0), mrb_fixnum_value(255));
	mrb_value bold = mrb_false_value();
	mrb_value italic = mrb_false_value();
	mrb_value shadow = mrb_false_value();
	mrb_value shadow_color = mrb_new_instance(mrb, "Color", 4,
			mrb_fixnum_value(0), mrb_fixnum_value(0), mrb_fixnum_value(0),
			mrb_fixnum_value(255));
	mrb_value underline = mrb_false_value();
	count = mrb_get_args(mrb, "|ooooooo", &size, &color, &bold, &italic,
			&shadow, &shadow_color, &underline);
	switch (count) {
	case 0:
		mrb_set_iv(mrb, self, "@size", size);
		mrb_set_iv(mrb, self, "@color", color);
		mrb_set_iv(mrb, self, "@bold", bold);
		mrb_set_iv(mrb, self, "@italic", italic);
		mrb_set_iv(mrb, self, "@shadow", shadow);
		mrb_set_iv(mrb, self, "@shadow_color", shadow_color);
		mrb_set_iv(mrb, self, "@underline", underline);
		break;
	case 1:
		mrb_set_iv(mrb, self, "@size", size);
		break;
	case 2:
		mrb_set_iv(mrb, self, "@size", size);
		mrb_set_iv(mrb, self, "@color", color);
		break;
	case 3:
		mrb_set_iv(mrb, self, "@size", size);
		mrb_set_iv(mrb, self, "@color", color);
		mrb_set_iv(mrb, self, "@bold", bold);
		break;
	case 4:
		mrb_set_iv(mrb, self, "@size", size);
		mrb_set_iv(mrb, self, "@color", color);
		mrb_set_iv(mrb, self, "@bold", bold);
		mrb_set_iv(mrb, self, "@italic", italic);
		break;
	case 5:
		mrb_set_iv(mrb, self, "@size", size);
		mrb_set_iv(mrb, self, "@color", color);
		mrb_set_iv(mrb, self, "@bold", bold);
		mrb_set_iv(mrb, self, "@italic", italic);
		mrb_set_iv(mrb, self, "@shadow", shadow);
		break;
	case 6:
		mrb_set_iv(mrb, self, "@size", size);
		mrb_set_iv(mrb, self, "@color", color);
		mrb_set_iv(mrb, self, "@bold", bold);
		mrb_set_iv(mrb, self, "@italic", italic);
		mrb_set_iv(mrb, self, "@shadow", shadow);
		mrb_set_iv(mrb, self, "@shadow_color", shadow_color);
		break;
	case 7:
		mrb_set_iv(mrb, self, "@size", size);
		mrb_set_iv(mrb, self, "@color", color);
		mrb_set_iv(mrb, self, "@bold", bold);
		mrb_set_iv(mrb, self, "@italic", italic);
		mrb_set_iv(mrb, self, "@shadow", shadow);
		mrb_set_iv(mrb, self, "@shadow_color", shadow_color);
		mrb_set_iv(mrb, self, "@underline", underline);
		break;
	default:
		mrb_raise(mrb, E_ARGUMENT_ERROR, "Wrong number of arguments");
		return mrb_nil_value();
		break;
	}
	return self;
}

void mruby_mrgsl_textdecorator_init(mrb_state *mrb) {
	struct RClass *type = mrb_define_class_under(mrb, mruby_get_mrgsl(mrb),
			"TextDecorator", mrb->object_class);
	mrb_define_method(mrb, type, "initialize", (mrb_func_t) initialize,
			MRB_ARGS_OPT(7));
	mrb_attr_accessor(mrb, type, "size");
	mrb_attr_accessor(mrb, type, "color");
	mrb_attr_accessor(mrb, type, "bold");
	mrb_attr_accessor(mrb, type, "italic");
	mrb_attr_accessor(mrb, type, "shadow");
	mrb_attr_accessor(mrb, type, "shadow_color");
	mrb_attr_accessor(mrb, type, "underline");

}
