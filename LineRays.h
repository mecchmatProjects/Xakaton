#ifndef _LINERAYS_H_
#define _LINERAYS_H_

#include "Types.h"

// Відстань від точки до прямої
PTYPE length_point_line(TPoint p, TLine line);

// Відстань від точки до відрізку
PTYPE length_point_segment(TPoint p, TSegment line);

// Відстань від точки до променя
PTYPE length_point_ray(TPoint p, TRay ray);

// Найменша відстань між двома відрізками
PTYPE length_segments(TSegment line1, TSegment line2);

// Точка перетину між променями (якщо ні - повертає NULL)
TPoint* intersect_rays(TRay r1, TRay r2);

// Точка перетину прямих (якщо ні - повертає NULL)
TPoint* intersect_lines(TLine l1, TLine l2);

// Точка перетину відрізків (якщо немає - повертає NULL)
TPoint* intersect_segments(TSegment s1, TSegment s2);

#endif // _LINERAYS_H_