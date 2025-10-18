#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

typedef unsigned NTYPE;
typedef float PTYPE;

#define TRUE  1
#define FALSE 0


typedef struct TPoint{
    PTYPE x;
    PTYPE y;
}TPoint;

typedef struct Polygone{
   NTYPE n; // power of Polynome
   TPoint *vertice;     
}Polygone;

// line: ax +by +c=0
typedef struct Line{
   PTYPE a;
   PTYPE b;
   PTYPE c;     
} TLine;


// line: ax +by +c=0
typedef struct Ray{
   TPoint start_point;
   TPoint direction_point;     
} TRay;

// Segemnt is given by two Points
typedef struct Segment{
   TPoint pointA;
   TPoint pointB;     
} TSegment;


// Segemnt is given by two Points
typedef struct Triangle{
   TPoint pointA;
   TPoint pointB;
   TPoint pointC;
} TTriangle;


#ifdef __cplusplus
}
#endif /// end of __cplusplus

#endif // end of TYPES_H

