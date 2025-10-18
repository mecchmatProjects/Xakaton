#pragma once
#include "Polygone.h"
#include "Types.h"

int testVectors(); // test all in 1 file they are simple

int testInputPolygone(); // 3 tests enough

int testWritePolygones(); // 3 tests

int testDeletePolygone(); // 1 test

int testShowPolygoneFile(); // 3 tests


//These all requires at least 3 tests each
extern int isPresentPolygone(FILE* fp, const Polygone* p);

 int isEqualPolygone(const Polygone* p1,const Polygone* p2);

 extern PTYPE perimeterPolygone(const Polygone* p);

 extern PTYPE areaPolygone(const Polygone* p);

 extern int isConvexPolygone(const Polygone* p);

 extern int maxPerimeterPolygone(FILE* fp, Polygone* p);

 extern int minAreaPolygone(FILE* fp, Polygone* p);

 extern NTYPE numberConvexPolygones(FILE* fp);

 extern NTYPE conditionPolygones(FILE* fp, predicatPolygone Q, const char* fname);

 extern NTYPE pointsPolygones(FILE* fp, TPoint p);

 extern NTYPE linePolygones(FILE* fp, TLine L);

 extern NTYPE rayPolygones(FILE* fp, TRay R);
// extern int test_isEqual();
// extern int test_isEqual2();
// extern int test_setVector();
// extern int test_lengthVector();
// extern int test_addVector();
// extern int test_multVector();
// extern int test_subVector();
// extern int test_scalarMultVector();
// extern int test_vectorMultVector();
// extern int test_area();
// extern int test_area_polygon();
// extern int test_inPolygon();
// extern int test_pointsPolygones(); // NTYPE у .c, але повертає int-сумісний TRUE/FALSE
// extern int test_isConvexPolygone_convex();
// extern int test_isConvexPolygone_nonConvex();
// extern int test_isConvexPolygone_triangle();
// extern int test_isConvexPolygone_invalid();
// extern int test_minAreaPolygone();
// extern int test_numberConvexPolygones();




