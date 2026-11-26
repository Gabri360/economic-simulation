#ifndef RENDER_H
#define RENDER_H


#include <SDL.h>
#include <SDL_ttf.h>
#include "const.h"

void draw_text(SDL_Renderer *ren, TTF_Font *font, const char *text, int x, int y, int px, SDL_Color color);

void draw_box(SDL_Renderer *ren, SDL_Window *win);

void draw_grid(SDL_Renderer *ren, SDL_Window *win);

void draw_circle(SDL_Renderer *ren, int x, int y, int r, SDL_Color color);

void draw_circle_f(SDL_Renderer *ren, float x, float y, float r, SDL_Color color);

void draw_pop(SDL_Renderer *ren, SDL_Window *win, const Pop *p, double alpha);

#endif
