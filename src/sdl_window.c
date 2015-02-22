/*
 * sdl_window.c
 *
 *  Created on: 7/12/2014
 *      Author: chronno
 */

#define GLEW_STATIC
#include <GL/glew.h>
#ifdef _WIN32
#include <GL/glu.h>
#elif __linux__
#include <GL/glut.h>
#endif
#include <mruby.h>
#include <mruby/variable.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
/* #include <SDL2/SDL_ttf.h> */
#include "mrgsl.h"
#include "tools.h"

mrgsl_sdl_window screen;

int
create_window (int width, int height, const char* title)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  /* TTF_Init(); */
  IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
  screen.width = width;
  screen.height = height;
  SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
  screen.window = SDL_CreateWindow (title, 0, 160, screen.width, screen.height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  screen.renderer = SDL_CreateRenderer (screen.window, -1, SDL_RENDERER_ACCELERATED);
  screen.context = SDL_GL_CreateContext (screen.window);
  glEnable (GL_TEXTURE_2D);
  glViewport (0, 0, width, height);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho (0, width, height, 0, -1, 1);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  return 0;
}

int
update_window (mrb_state* mrb)
{
  glClearColor (1.0f, 1.f, 0.0f, 1.f);
  glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  SDL_PollEvent (&screen.event);
  if (screen.event.type == SDL_QUIT)
    {
      SDL_DestroyWindow (screen.window);
      mrb_set_gv(mrb, "$running", mrb_false_value());
      SDL_Quit ();
    }
  else
    {
      mrgsl_draw_viewport(mrb, get_graphics_viewport(mrb));
      glFlush ();
      SDL_GL_SwapWindow (screen.window);
    }
  return 1;
}

mrb_int
window_width ()
{
  return screen.width;
}

mrb_int
window_height ()
{
  return screen.height;
}
