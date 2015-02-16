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

static void font_free(mrb_state *mrb, void *p) {
	if (p) {
		mrgsl_font* font = (mrgsl_font*) p;
		SDL_free(font->font);
		free(p);
	}
}
struct mrb_data_type const mrbal_font_data_type = { "mrgsl/Font", font_free };

static mrb_value initialize(mrb_state* mrb, mrb_value self) {
	const char* name;
	mrb_value fontname;
	mrb_int size;
	mrb_int count;
	mrb_value zero = mrb_fixnum_value(0);
	mrb_value full = mrb_fixnum_value(255);
	mrgsl_font* font = (mrgsl_font*) malloc(sizeof(mrgsl_font));
	count = mrb_get_args(mrb, "o|i", &fontname, &size);
	if (count == 1 && mrb_string_p(fontname)) {
		name = mrb_string_value_ptr(mrb, fontname);
		font->font = TTF_OpenFont(name, 12);
		if (!font->font) {
			printf("TTF_OpenFont: %s\n", TTF_GetError());
			mrb_raise(mrb, E_ARGUMENT_ERROR, "Asset not found");
			return mrb_nil_value();
		}
		mrb_set_iv(mrb, self, "@name", fontname);
		mrb_set_iv(mrb, self, "@size", mrb_fixnum_value(12));
	} else if (count == 2 && mrb_string_p(fontname)) {
		name = mrb_string_value_ptr(mrb, fontname);
		font->font = TTF_OpenFont(name, 12);
		if (!font->font) {
			mrb_raise(mrb, E_ARGUMENT_ERROR, "Asset not found");
			return mrb_nil_value();
		}
		mrb_set_iv(mrb, self, "@name", fontname);
		mrb_set_iv(mrb, self, "@size", mrb_fixnum_value(size));
	} else {
		mrb_raise(mrb, E_ARGUMENT_ERROR, "Wrong type of arguments");
		return mrb_nil_value();
	}

	mrb_set_iv(mrb, self, "@color",
			mrb_new_instance(mrb, "Color", 4, zero, zero, zero, full));
	mrb_set_iv(mrb, self, "@bold", mrb_false_value());
	mrb_set_iv(mrb, self, "@italic", mrb_false_value());
	mrb_set_iv(mrb, self, "@underline", mrb_false_value());
	DATA_TYPE (self) = &mrbal_font_data_type;
	DATA_PTR (self) = font;
	return self;
}

static mrb_value set_bold(mrb_state *mrb, mrb_value self) {
	/* Variables */
	int style;
	mrb_bool value;
	TTF_Font *font;
	/* Esto usa un argumento, de tipo booleano */
	mrb_get_args(mrb, "b", &value);
	/* Obtiene la estructura de C */
	font = DATA_PTR(self);
	/* Obtenemos el estilo anterior */
	style = TTF_GetFontStyle(font);
	/* Si la queres bold */
	if (value) {
		TTF_SetFontStyle(font, style | TTF_STYLE_BOLD);
	} else { /* si no la queres bold */
		TTF_SetFontStyle(font, style & (~TTF_STYLE_BOLD));
	};
	mrb_set_iv(mrb, self, "@bold", mrb_bool_value(value));
	return value ? mrb_true_value() : mrb_false_value();
}

static mrb_value set_italic(mrb_state *mrb, mrb_value self) {
	/* Variables */
	int style;
	mrb_bool value;
	TTF_Font *font;
	/* Esto usa un argumento, de tipo booleano */
	mrb_get_args(mrb, "b", &value);
	/* Obtiene la estructura de C */
	font = DATA_PTR(self);
	/* Obtenemos el estilo anterior */
	style = TTF_GetFontStyle(font);
	/* Si la queres bold */
	if (value) {
		TTF_SetFontStyle(font, style | TTF_STYLE_ITALIC);
	} else { /* si no la queres bold */
		TTF_SetFontStyle(font, style & (~TTF_STYLE_ITALIC));
	};
	mrb_set_iv(mrb, self, "@italic", mrb_bool_value(value));
	return value ? mrb_true_value() : mrb_false_value();
}
;

static mrb_value set_underline(mrb_state *mrb, mrb_value self) {
	/* Variables */
	int style;
	mrb_bool value;
	TTF_Font *font;
	/* Esto usa un argumento, de tipo booleano */
	mrb_get_args(mrb, "b", &value);
	/* Obtiene la estructura de C */
	font = DATA_PTR(self);
	/* Obtenemos el estilo anterior */
	style = TTF_GetFontStyle(font);
	/* Si la queres bold */
	if (value) {
		TTF_SetFontStyle(font, style | TTF_STYLE_UNDERLINE);
	} else { /* si no la queres bold */
		TTF_SetFontStyle(font, style & (~TTF_STYLE_UNDERLINE));
	};
	mrb_set_iv(mrb, self, "@underline", mrb_bool_value(value));
	return value ? mrb_true_value() : mrb_false_value();
}
;

static mrb_value set_strike(mrb_state *mrb, mrb_value self) {
	/* Variables */
	int style;
	mrb_bool value;
	TTF_Font *font;
	/* Esto usa un argumento, de tipo booleano */
	mrb_get_args(mrb, "b", &value);
	/* Obtiene la estructura de C */
	font = DATA_PTR(self);
	/* Obtenemos el estilo anterior */
	style = TTF_GetFontStyle(font);
	/* Si la queres bold */
	if (value) {
		TTF_SetFontStyle(font, style | TTF_STYLE_STRIKETHROUGH);
	} else { /* si no la queres bold */
		TTF_SetFontStyle(font, style & (~TTF_STYLE_STRIKETHROUGH));
	};
	mrb_set_iv(mrb, self, "@strike", mrb_bool_value(value));
	return value ? mrb_true_value() : mrb_false_value();
}
;

static mrb_value dispose(mrb_state *mrb, mrb_value self) {
	TTF_Font* font; //creo la variable font
	font = DATA_PTR(self); //asigno variable front la estructura de C que esta en el objeto
	TTF_CloseFont(font); // CloseFont liberla la memoria donde estaba la fuente
	font = NULL; //hago que la variable no tenga nada
	return mrb_nil_value(); //devuelvo a ruby un nulo
}

void mruby_mrgsl_font_init(mrb_state *mrb) {
	struct RClass *type = mrb_define_class_under(mrb, mruby_get_mrgsl(mrb),
			"Font", mrb->object_class);
	MRB_SET_INSTANCE_TT(type, MRB_TT_DATA);
	mrb_define_method(mrb, type, "initialize", (mrb_func_t) initialize,
	MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
	mrb_define_method(mrb, type, "bold=", (mrb_func_t) set_bold,
			MRB_ARGS_REQ(1));
	mrb_define_method(mrb, type, "italic=", (mrb_func_t) set_italic,
			MRB_ARGS_REQ(1));
	mrb_define_method(mrb, type, "underline=", (mrb_func_t) set_underline,
			MRB_ARGS_REQ(1));
	mrb_define_method(mrb, type, "strike=", (mrb_func_t) set_strike,
			MRB_ARGS_REQ(1));
	mrb_define_method(mrb, type, "dispose", (mrb_func_t) dispose,
	MRB_ARGS_NONE());
	mrb_attr_reader(mrb, type, "name");
	mrb_attr_reader(mrb, type, "size");
	mrb_attr_accessor(mrb, type, "color");
	mrb_attr_reader(mrb, type, "bold");
	mrb_attr_reader(mrb, type, "italic");
	mrb_attr_reader(mrb, type, "underline");
	mrb_attr_reader(mrb, type, "strike");

}
