#ifndef _POLYGONE_H_
#define _POLYGONE_H_

#include "stdio.h"
#include "stdlib.h"

#include "Types.h"
#include "Predicat.h"


// Ввести багатокутник з консолі (якщо fp == NULL) або з текстового файлу
extern int inputPolygone(Polygone* p, FILE* fp);

// Вивести багатокутник в консоль (якщо fp == NULL) або в текстовий файл
extern void outputPolygone(const Polygone* p, FILE* fp);

// Звільнити пам'ять, виділену під вершини багатокутника
extern int freePolygone(Polygone* p);

// === Функції для роботи з файлами (бінарними та текстовими) ===

// Записати один багатокутник у бінарний файл
extern int writePolygone_binary(FILE* fp, Polygone* p);

// Записати один багатокутник у текстовий файл
extern int writePolygone_text(FILE* fp, Polygone* p);

// Видалити k-й багатокутник з бінарного файлу
extern int deletePolygonesFile(const char* fname, NTYPE k);

// Показати k-й багатокутник з файлу
extern void showPolygoneFile(FILE* fp, NTYPE k);

// Показати всі багатокутники з файлу
extern void showPolygonesFile(FILE* fp);

// === Функції порівняння та пошуку ===

// Перевірити, чи два багатокутники є однаковими
extern int isEqualPolygone(const Polygone* p1, const Polygone* p2);

// Перевірити, чи присутній даний багатокутник у файлі
extern int isPresentPolygone(FILE* fp, const Polygone* p);

// === Базові геометричні обчислення ===

// Обчислити периметр багатокутника
extern PTYPE perimeterPolygone(const Polygone* p);

// Обчислити площу багатокутника (за формулою Гаусса)
extern PTYPE area_polygon(const Polygone* p);

// Перевірити, чи є багатокутник опуклим
extern int isConvexPolygone(const Polygone* p);

// === Функції для аналізу набору багатокутників у файлі ===

// Знайти багатокутник з максимальним периметром
extern int maxPerimeterPolygone(FILE* fp, Polygone* p);

// Знайти багатокутник з мінімальною площею
extern int minAreaPolygone(FILE* fp, Polygone* p);

// Порахувати кількість опуклих багатокутників у файлі
extern NTYPE numberConvexPolygones(FILE* fp);

// Відфільтрувати багатокутники з файлу А у файл В за умовою Q
extern NTYPE conditionPolygones(FILE* fp, predicatPolygone Q, const char* fname);

// Порахувати кількість багатокутників, що містять точку Р
extern NTYPE pointsPolygones(FILE* fp, TPoint p);

// === Просунута геометрія (пункт "к") ===

// Чи міститься точка Р всередині багатокутника (навіть неопуклого)
extern int pointsPolygoneInside(const Polygone* p, TPoint p0);

// Чи є багатокутник Трикутником?
extern int isTrianglePolygone(const Polygone* p);

// Чи є багатокутник Чотирикутником?
extern int isQuadrilateralPolygone(const Polygone* p);

// Триангуляція багатокутника (повертає кількість трикутників)
extern NTYPE triangulatePolygone(const Polygone* p1, TTriangle* tr);

// Чи перетинаються два багатокутника
extern int isIntersectPolygones(const Polygone* p1, const Polygone* p2);

// Знайти багатокутник, що утворений перетином двох багатокутників
extern Polygone intersect_polygone(const Polygone* p1, const Polygone* p2);

// Знайти найменший опуклий багатокутник, що містить даний
extern Polygone convex_wrapper(const Polygone* p1);

// Знайти багатокутник, що знаходиться на відстані dist від даного
extern Polygone wrapper_distance(const Polygone* p1, PTYPE dist);

#endif // _POLYGONE_H_