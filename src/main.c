#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "render.h"
#include "logic.h"
#include "const.h"



int main(int argc, char *argv[]) {

	(void)argc; (void)argv;

	//----------------------------------INIT
	srand((unsigned)time(NULL));
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *win = SDL_CreateWindow("eco_sim", SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED, 1920*SCALE, 1080*SCALE,
                                       SDL_WINDOW_RESIZABLE);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED |  SDL_RENDERER_PRESENTVSYNC);
    if (!win || !ren) {
        fprintf(stderr, "SDL: %s\n", SDL_GetError());
        return 1;
    }

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

	char font_path[1024];
    char *base = SDL_GetBasePath();
    snprintf(font_path, sizeof font_path, "%s%s", base ? base : "", FONT_FILE);
    SDL_free(base);
	if (TTF_Init() != 0) {
		fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
		return 1;
	}
	TTF_Font *font = TTF_OpenFont(font_path, 16);
	if (!font) {
		fprintf(stderr, "TTF_OpenFont (%s): %s\n", font_path, TTF_GetError());
		return 1;
	}

	Pop pop;
	size_t n = 50;
	pop_init(&pop, n);
	pop_set(&pop);

	const Uint64 freq = SDL_GetPerformanceFrequency();
	const Uint64 max_ticks = (Uint64)(MAX_FRAME_SECONDS * (double)freq);
	Uint64 prev = SDL_GetPerformanceCounter();
	Uint64 acc = 0;
	double speed = 0.0625;
	int paused = 0;
	unsigned long steps = 0;

    int running = 1;

	//--------------------------------LOOP
    while (running) {

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
            if (e.type != SDL_KEYDOWN || e.key.repeat) continue;
            switch (e.key.keysym.sym) {
            case SDLK_ESCAPE:
            case SDLK_q:
                running = 0;
                break;
            case SDLK_SPACE:
                paused = !paused;
                break;
            case SDLK_UP:
                if (speed < SIM_SPEED_MAX) { speed *= 2.0; acc = 0; }
                break;
            case SDLK_DOWN:
                if (speed > SIM_SPEED_MIN) { speed /= 2.0; acc = 0; }
                break;
            default:
                break;
            }
        }

		//--------------------------SIMULATION

		Uint64 now = SDL_GetPerformanceCounter();
		Uint64 dt = now - prev;
		prev = now;

		if (!paused) {
			const Uint64 step_ticks = (Uint64)((double)freq / (SIM_HZ * speed));
			if (dt > max_ticks) dt = max_ticks;
			acc += dt;
			while (acc >= step_ticks) {
				step(&pop);
				acc -= step_ticks;
				steps++;
			}
		}

		//-----------------------------PLOT

		draw_box(ren, win);
		draw_grid(ren, win);
		draw_pop(ren, win, &pop);

		char hud[128];
		snprintf(hud, sizeof hud, "%s | step %lu | %.2gx (%d Hz) | SPAZIO=pausa UP/DOWN=velocita | POP = %ld",
		         paused ? "PAUSA" : "RUN", steps, speed, (int)(SIM_HZ * speed), n);
		draw_text(ren, font, hud, padding + 10, padding + 10, 18,
		          (SDL_Color){180, 255, 190, 255});

		SDL_RenderPresent(ren);
	}

	TTF_CloseFont(font);
    TTF_Quit();
	pop_free(&pop);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
