/*
 * mrgsl.h
 *
 *  Created on: 7/12/2014
 *      Author: chronno
 */
#ifndef MRGSL_MRGSL_H_
#define MRGSL_MRGSL_H_

#include <GL/gl.h>
#include <mruby.h>
#include <mruby/value.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

extern struct mrb_data_type const mrbal_bitmap_data_type;
extern struct mrb_data_type const mrbal_viewport_data_type;

extern void
mrgsl_destroy();

typedef struct {
	SDL_Surface *surface;
	GLuint texture;
} mrgsl_bitmap;

typedef struct {
	mrb_value *children;
	mrb_int size;
	mrb_int capacity;
} mrgsl_viewport;

void
mruby_mrgsl_init(mrb_state *mrb);
struct RClass*
mruby_get_mrgsl(mrb_state *mrb);
void
mruby_mrgsl_point_init(mrb_state *mrb);
void
mruby_mrgsl_rect_init(mrb_state *mrb);
void
mruby_mrgsl_color_init(mrb_state *mrb);
void
mruby_mrgsl_bitmap_init(mrb_state *mrb);
void
mruby_mrgsl_textdecorator_init(mrb_state *mrb);
void
mruby_mrgsl_sprite_init(mrb_state *mrb);
void
mruby_mrgsl_viewport_init(mrb_state *mrb);
void
mruby_mrgsl_keyboard_init(mrb_state *mrb);
void
mruby_mrgsl_mouse_init(mrb_state* mrb);
void
mruby_mrgsl_graphics_init(mrb_state *mrb);
void
mruby_kernel_require(mrb_state *mrb);

/*
 * SDL Functions
 */

typedef struct {
	int width;
	int height;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_GLContext context;
	SDL_Event event;
} mrgsl_sdl_window;

typedef struct {
	int size;
	int capacity;
	mrb_value* drawables;
} mrgsl_graphics_container;

int
create_window(int width, int height, const char* title);
int
update_window(mrb_state* mrb);
mrb_int
window_width();
mrb_int
window_height();
SDL_Surface *
create_surface(mrb_int width, mrb_int height);
SDL_Surface *
load_surface(const char* filename);
GLuint
surface_texture(SDL_Surface *surface);
mrb_int
surface_height(SDL_Surface *surface);
mrb_int
surface_width(SDL_Surface *surface);
#endif /* MRGSL_MRGSL_H_ */
