#pragma once

#include "Types.h"
#include "vectors.h"

extern int isEqual(PTYPE a, PTYPE b);

extern int isEqual2(TVECT a, TVECT b);

typedef int (*predicatInt)(int);

typedef int (*predicatDouble)(double);

typedef int (*predicatPolygone)(const Polygone* p);

