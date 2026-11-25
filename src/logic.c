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
    p->skill = calloc(n, sizeof *p->skill);
	p->x = calloc(n, sizeof *p->x);
	p->y = calloc(n, sizeof *p->y);
}

void pop_set(Pop *p)
{
	for(size_t i=0;i<p->n;i++) {
		p->money[i] = 1000;
		p->skill[i] = myrand();
		p->x[i] = uniform();
		p->y[i] = uniform();
	}
}

static void pop_copy(const Pop *src, Pop *dst)
{
	for(size_t i=0;i<src->n;i++) {
		dst->money[i] = src->money[i];
		dst->skill[i] = src->skill[i];
	}
}

void pop_free(Pop *p)
{
    free(p->money);
    free(p->skill);
	free(p->x);
	free(p->y);
    p->money = NULL;
    p->skill = NULL;
	p->x = NULL;
    p->y = NULL;
}


void step(Pop *p)
{
	Pop cp;
	pop_init(&cp, p->n);
	if (!cp.money || !cp.skill) {
		pop_free(&cp);
		return;
	}
	pop_copy(p, &cp);

	double total = 0.0;
	for(size_t i=0;i<p->n;i++) total += cp.money[i];

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

	free(next);
	pop_free(&cp);
}
