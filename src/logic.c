#include "logic.h"
#include <math.h>
#include <stdlib.h>



static double uniform(void)
{
    return (rand() + 0.5) / ((double)RAND_MAX + 1.0);
}

static double myrand(void)
{
    const double mu = 0.5;
    const double sigma = 0.15;

    static int has_spare = 0;
    static double spare;

    if (has_spare) {
        has_spare = 0;
        return mu + sigma * spare;
    }

    double u1 = uniform();
    double u2 = uniform();
    double r = sqrt(-2.0 * log(u1));

    spare = r * sin(2.0 * M_PI * u2);
    has_spare = 1;
	double x = mu + sigma * (r * cos(2.0 * M_PI * u2));
	if(x<0) x=0;
	if(x>1) x=1;
    return x;
}



void pop_init(Pop *p, size_t n)
{
	p->n = n;
    p->money = calloc(n, sizeof *p->money);
    p->prev_money = calloc(n, sizeof *p->prev_money);
    p->skill = calloc(n, sizeof *p->skill);
	p->x = calloc(n, sizeof *p->x);
	p->y = calloc(n, sizeof *p->y);
	p->prev_x = calloc(n, sizeof *p->prev_x);
	p->prev_y = calloc(n, sizeof *p->prev_y);
	p->x0 = calloc(n, sizeof *p->x0);
	p->y0 = calloc(n, sizeof *p->y0);
	p->phase_x = calloc(n, sizeof *p->phase_x);
	p->phase_y = calloc(n, sizeof *p->phase_y);
}

void pop_set(Pop *p)
{
	for(size_t i=0;i<p->n;i++) {
		p->money[i] = 1000;
		p->prev_money[i] = p->money[i];
		p->skill[i] = myrand();
		p->x0[i] = uniform();
		p->y0[i] = uniform();
		p->phase_x[i] = uniform() * 2.0 * M_PI;
		p->phase_y[i] = uniform() * 2.0 * M_PI;

		/* stato iniziale coerente con la formula usata da step():
		   senza questo, il primo step sposta tutti i punti di POS_NOISE. */
		double nx = p->x0[i] + POS_NOISE * sin(p->phase_x[i]);
		double ny = p->y0[i] + POS_NOISE * sin(p->phase_y[i]);
		p->x[i] = nx < 0.0 ? 0.0 : (nx > 1.0 ? 1.0 : nx);
		p->y[i] = ny < 0.0 ? 0.0 : (ny > 1.0 ? 1.0 : ny);
		p->prev_x[i] = p->x[i];
		p->prev_y[i] = p->y[i];
	}
}

static void pop_copy(const Pop *src, Pop *dst)
{
	for(size_t i=0;i<src->n;i++) {
		dst->money[i] = src->money[i];
		dst->prev_money[i] = src->prev_money[i];
		dst->skill[i] = src->skill[i];
		dst->x[i] = src->x[i];
		dst->y[i] = src->y[i];
		dst->prev_x[i] = src->prev_x[i];
		dst->prev_y[i] = src->prev_y[i];
		dst->x0[i] = src->x0[i];
		dst->y0[i] = src->y0[i];
		dst->phase_x[i] = src->phase_x[i];
		dst->phase_y[i] = src->phase_y[i];
	}
}

void pop_free(Pop *p)
{
    free(p->money);
    free(p->prev_money);
    free(p->skill);
	free(p->x);
	free(p->y);
	free(p->prev_x);
	free(p->prev_y);
	free(p->x0);
	free(p->y0);
	free(p->phase_x);
	free(p->phase_y);
    p->money = NULL;
    p->prev_money = NULL;
    p->skill = NULL;
	p->x = NULL;
    p->y = NULL;
	p->prev_x = NULL;
	p->prev_y = NULL;
	p->x0 = NULL;
	p->y0 = NULL;
	p->phase_x = NULL;
	p->phase_y = NULL;
}


void step(Pop *p)
{
	if (!p->money || !p->prev_money || !p->skill ||
	    !p->x || !p->y || !p->prev_x || !p->prev_y || !p->x0 || !p->y0 ||
	    !p->phase_x || !p->phase_y) return;

	Pop cp;
	pop_init(&cp, p->n);
	if (!cp.money || !cp.prev_money || !cp.skill || !cp.x || !cp.y ||
	    !cp.prev_x || !cp.prev_y || !cp.x0 || !cp.y0 || !cp.phase_x || !cp.phase_y) {
		pop_free(&cp);
		return;
	}
	pop_copy(p, &cp);

	double total = 0.0;
	for(size_t i=0;i<p->n;i++) {
		p->prev_money[i] = cp.money[i];
		p->prev_x[i] = cp.x[i];
		p->prev_y[i] = cp.y[i];
		total += cp.money[i];
	}

	double *next = malloc(p->n * sizeof *next);
	if (!next) {
		pop_free(&cp);
		return;
	}

	double sum = 0.0;
	for(size_t i=0;i<p->n;i++) {
		double r = SKILL_SCALE * cp.skill[i] + (uniform() - 0.5) * FRT_SCALE;
		double growth = 1.0 + r;
		if (growth < 0.0) growth = 0.0;

		next[i] = cp.money[i] * growth + REPLICATOR_SEED;
		sum += next[i];
	}

	if (sum > 0.0) {
		double scale = total / sum;
		for(size_t i=0;i<p->n;i++) p->money[i] = next[i] * scale;
	}

	for(size_t i=0;i<p->n;i++) {
		p->phase_x[i] = cp.phase_x[i] + POS_NOISE_SPEED;
		p->phase_y[i] = cp.phase_y[i] + POS_NOISE_SPEED * 1.37;

		double nx = cp.x0[i] + POS_NOISE * sin(p->phase_x[i]);
		double ny = cp.y0[i] + POS_NOISE * sin(p->phase_y[i]);
		p->x[i] = nx < 0.0 ? 0.0 : (nx > 1.0 ? 1.0 : nx);
		p->y[i] = ny < 0.0 ? 0.0 : (ny > 1.0 ? 1.0 : ny);
	}

	free(next);
	pop_free(&cp);
}
