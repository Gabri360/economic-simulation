#ifndef CONST_H
#define CONST_H

#define M_PI 3.14159265358979323846

#define SCALE 0.9

#define FONT_FILE "assets/DejaVuSansMono.ttf"

#define WIDTH 50
#define HEIGHT (int)(WIDTH*1080/1920)
#define padding 20

#define FRT_SCALE 0.5
#define SKILL_SCALE 0.1
#define MONEY_SCALE 0.1
#define REPLICATOR_SEED 0.1


#define POS_NOISE 0.01
#define POS_NOISE_SPEED 0.015

#define SIM_HZ 60
#define MAX_FRAME_SECONDS 0.25
#define SIM_SPEED_MIN 0.0625
#define SIM_SPEED_MAX 8.0

#include <stddef.h>
typedef struct {
	size_t n;
	double *money;
	double *prev_money;
	double *skill;
	double *x;
	double *y;
	double *prev_x;
	double *prev_y;
	double *x0;
	double *y0;
	double *phase_x;
	double *phase_y;
} Pop;

#endif
