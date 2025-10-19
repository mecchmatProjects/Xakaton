#pragma once

#include "Types.h"
#include "vectors.h"
#include <math.h>

#define EPS 1e-9 // Accuracy for comparison // Для точності

extern int isEqual(PTYPE a, PTYPE b);

extern int isEqual2(TVECT a, TVECT b);

typedef int (*predicatInt)(int);

typedef int (*predicatDouble)(double);

typedef int (*predicatPolygone)(const Polygone* p);


// Tests whether two floating-point numbers are equal to the specified precision // Перевірка для чисел з плаваючою крапкою
// static inline int isEqual(PTYPE a, PTYPE b) {
//     return fabs(a - b) < EPS;
// }