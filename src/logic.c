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
}

void pop_set(Pop *p)
{
	for(size_t i=0;i<p->n;i++) {
		p->money[i] = 1000;
		p->skill[i] = myrand();
	}
}

void pop_free(Pop *p)
{
    free(p->money);
    free(p->skill);
}


void step(Pop *p)
{
	double frt, change;
	for(size_t i=0;i<p->n;i++) {
		frt = uniform() * FRT_SCALE;
		for(size_t k=0;i<p->n;i++) {
			change = p->money[k] * p->skill[i] * SKILL_SCALE + frt * p->money[k] + p->money[i]/(double)(1000 * p->n) * MONEY_SCALE;
			if (change>p->money[k]) change = p->money[k];

			p->money[i] += change;
			p->money[k] -= change;
		}
	}
}
