#include "render.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


void draw_text(SDL_Renderer *ren, TTF_Font *font, const char *text, int x, int y, int px, SDL_Color color)
{
    TTF_SetFontSize(font, px);
    SDL_Surface *surf = TTF_RenderUTF8_Blended(font, text, color);
    if (!surf) return;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surf);
    SDL_Rect dst = { x, y, surf->w, surf->h };
    SDL_FreeSurface(surf);
    if (!tex) return;
    SDL_RenderCopy(ren, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
}

void draw_box(SDL_Renderer *ren, SDL_Window *win)
{
	int w, h;
	SDL_GetWindowSize(win, &w, &h);

	SDL_SetRenderDrawColor(ren, 20, 20, 25, 255);
	SDL_RenderClear(ren);

	SDL_SetRenderDrawColor(ren, 50, 50, 60, 255);
	SDL_Rect r = {padding, padding, w - 2 * padding , h - 2 * padding };
	SDL_RenderFillRect(ren, &r);
	SDL_SetRenderDrawColor(ren, 50, 150, 60, 255);
	SDL_RenderDrawRect(ren, &r);
}

void draw_grid(SDL_Renderer *ren, SDL_Window *win)
{

	int w, h;
	SDL_GetWindowSize(win, &w, &h);

	SDL_SetRenderDrawColor(ren, 50, 150, 60, 100);
	int cell_width = (w - 2*padding);
	int cell_height = (h - 2*padding);

	int x;
	for(int i=1;i<WIDTH;i++) {
		x = padding + (int)(((long long)cell_width*i)/WIDTH);
		SDL_RenderDrawLine(ren, x, padding, x, h - padding);
	}
	for(int i=1;i<HEIGHT;i++) {
		x = padding + (int)(((long long)cell_height*i)/HEIGHT);
		SDL_RenderDrawLine(ren, padding, x, w - padding, x);
	}

}

void draw_circle(SDL_Renderer *ren, int x, int y, int r, SDL_Color color)
{
    if (r < 0) return;

    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);

    int cx = 0;
    int cy = r;
    int d = 3 - 2 * r;

    while (cx <= cy) {
        SDL_RenderDrawPoint(ren, x + cx, y + cy);
        SDL_RenderDrawPoint(ren, x - cx, y + cy);
        SDL_RenderDrawPoint(ren, x + cx, y - cy);
        SDL_RenderDrawPoint(ren, x - cx, y - cy);
        SDL_RenderDrawPoint(ren, x + cy, y + cx);
        SDL_RenderDrawPoint(ren, x - cy, y + cx);
        SDL_RenderDrawPoint(ren, x + cy, y - cx);
        SDL_RenderDrawPoint(ren, x - cy, y - cx);

        if (d < 0) {
            d += 4 * cx + 6;
        } else {
            d += 4 * (cx - cy) + 10;
            cy--;
        }
        cx++;
    }
}

void draw_circle_f(SDL_Renderer *ren, float x, float y, float r, SDL_Color color)
{
    if (r < 0.0f) return;

    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);

    int segments = (int)(2.0f * (float)M_PI * r);
    if (segments < 8) segments = 8;
    if (segments > 4096) segments = 4096;

    float px = x + r;
    float py = y;
    for(int s = 1; s <= segments; s++) {
        float a = 2.0f * (float)M_PI * (float)s / (float)segments;
        float nx = x + r * cosf(a);
        float ny = y + r * sinf(a);
        SDL_RenderDrawLineF(ren, px, py, nx, ny);
        px = nx;
        py = ny;
    }
}

void draw_pop(SDL_Renderer *ren, SDL_Window *win, const Pop *p, double alpha)
{
	if (alpha < 0.0) alpha = 0.0;
	if (alpha > 1.0) alpha = 1.0;

	int w, h;
	SDL_GetWindowSize(win, &w, &h);
	double boxw = (double)(w - 2*padding);
	double boxh = (double)(h - 2*padding);

	for(size_t i = 0; i<p->n;i++) {
		double money = p->prev_money[i] + (p->money[i] - p->prev_money[i]) * alpha;
		double px = p->prev_x[i] + (p->x[i] - p->prev_x[i]) * alpha;
		double py = p->prev_y[i] + (p->y[i] - p->prev_y[i]) * alpha;
		float x = padding + (float)(px * boxw);
		float y = padding + (float)(py * boxh);
		float r = (float)(money / ((double)p->n * 1.0));
		draw_circle_f(ren, x, y, r, (SDL_Color){180, 255, 190, 255});
	}
}
