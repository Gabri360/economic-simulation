#ifndef LOGIC_H
#define LOGIC_H

#include "const.h"

void pop_init(Pop *p, size_t n);

void pop_set(Pop *p);

void pop_free(Pop *p);

void step(Pop *p);
#endif
