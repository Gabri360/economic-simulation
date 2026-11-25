#ifndef CONST_H
#define CONST_H

#define M_PI 3.14159265358979323846

#define SCALE 0.9

#define FONT_FILE "assets/DejaVuSansMono.ttf"

#define WIDTH 50
#define HEIGHT (int)(WIDTH*1080/1920)
#define padding 20

#define FRT_SCALE 0.1
#define SKILL_SCALE 0.5
#define MONEY_SCALE 100

#include <stddef.h>
typedef struct {
	size_t n;
	double *money;
	double *skill;
} Pop;

#endif
