#pragma once
#include "Polygone.h"
#include "Types.h"
#include <stdio.h> // Потрібен для FILE*

// --- 1. testsPolygone.c (vectors) ---
extern int test_suite_isEqual();
extern int test_suite_setVector();
extern int test_suite_lengthVector();
extern int test_suite_add_sub_Vector();
extern int test_suite_multVector();
extern int test_suite_scalarMultVector();
extern int test_suite_vectorMultVector();

// --- 2. testsPolygone.c (Polygone geometry) ---
extern int test_suite_area_polygon();
extern int test_suite_inPolygon();
extern int test_suite_isConvexPolygone();
extern int test_suite_isIntersectPolygones();
extern int test_suite_convex_wrapper();

// --- 3. testsPolygone.c (Polygone file analysis) ---
extern int test_suite_isEqualPolygone();
extern int test_suite_isPresentPolygone();
extern int test_suite_deletePolygonesFile();
extern int test_suite_maxPerimeterPolygone();
extern int test_suite_minAreaPolygone();
extern int test_suite_numberConvexPolygones();
extern int test_suite_pointsPolygones();

// --- 4. testsPolygone.c (Predicat) ---
extern int test_suite_conditionPolygones();

// --- 5. testsPolygone.c (LineRays) ---
extern int test_suite_length_point_ray();
extern int test_suite_length_segments();
extern int test_suite_linePolygones();
extern int test_suite_rayPolygones();

// --- 6. testsPolygone.c (I/O) ---
extern int test_showPolygoneFile_suite();
extern int testInputPolygone();
extern int testWritePolygones();


extern NTYPE test_pointsPolygones();
extern int testShowPolygoneFile();

// --- Оголошення функцій, потрібних для тестів ---
extern int isEqualPolygone(const Polygone* p1,const Polygone* p2);
extern PTYPE perimeterPolygone(const Polygone* p);
extern int maxPerimeterPolygone(FILE* fp, Polygone* p);
extern NTYPE linePolygones(FILE* fp, TLine L);
extern NTYPE rayPolygones(FILE* fp, TRay R);