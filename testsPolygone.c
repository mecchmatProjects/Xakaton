#include "testsPolygone.h"
#include "Polygone.h"
#include "vectors.h"
#include <stdlib.h>
#include <assert.h> // Потрібен для assert()
#include "LineRays.h"
#include <stdio.h>

//
// === 1. ВЕКТОРНА АЛГЕБРА ===
//

int test_suite_isEqual() {
    int pass = 0;
    if (isEqual(1.0f, 1.0f) == TRUE) pass++;
    if (isEqual(1.0f, 1.5f) == FALSE) pass++;
    if (isEqual(1.0f, 1.0000001f) == TRUE) pass++;

    TVECT v1 = {1.f, 2.f, 3.f};
    TVECT v2 = {1.f, 2.f, 3.f};
    TVECT v3 = {0.f, 2.f, 3.f};
    if (isEqual2(v1, v2) == TRUE) pass++;
    if (isEqual2(v1, v3) == FALSE) pass++;

    return pass == 5;
}

int test_suite_setVector() {
    int pass = 0;
    TPoint p1 = {0.f, 0.f};
    TPoint p2 = {1.f, 1.f};
    TPoint p3 = {-2.f, 3.f};
    TVECT v1 = {1.f, 1.f, 0.f};
    TVECT v2 = {-3.f, 2.f, 0.f};
    if (isEqual2(setVector(p1, p2), v1)) pass++;
    if (isEqual2(setVector(p2, p3), v2)) pass++;
    if (isEqual2(setVector(p1, p1), (TVECT){0,0,0})) pass++;
    return pass == 3;
}

int test_suite_lengthVector() {
    int pass = 0;
    if (isEqual(lengthVector((TVECT){3.f, 4.f, 0.f}), 5.0f)) pass++;
    if (isEqual(lengthVector((TVECT){0.f, 0.f, 0.f}), 0.0f)) pass++;
    if (isEqual(lengthVector((TVECT){-1.f, 0.f, 0.f}), 1.0f)) pass++;
    return pass == 3;
}

int test_suite_add_sub_Vector() {
    int pass = 0;
    TVECT v1 = {3.f, 4.f, 0.f};
    TVECT v2 = {-3.f, -4.f, 0.f};
    TVECT v3 = {0.f, 0.f, 0.f};
    if (isEqual2(addVector(v1, v2), v3)) pass++;
    if (isEqual2(subVector(v1, v1), v3)) pass++;
    if (isEqual2(subVector(v3, v1), v2)) pass++;
    return pass == 3;
}

int test_suite_multVector() {
    int pass = 0;
    TVECT v1 = {3.f, 4.f, 0.f};
    TVECT v2 = {0.f, 0.f, 0.f};
    TVECT v3 = {6.f, 8.f, 0.f};
    if (isEqual2(multVector(0.0f, v1), v2)) pass++;
    if (isEqual2(multVector(2.0f, v1), v3)) pass++;
    if (isEqual2(multVector(-1.0f, v1), (TVECT){-3.f, -4.f, 0.f})) pass++;
    return pass == 3;
}

int test_suite_scalarMultVector() {
    int pass = 0;
    TVECT v1 = {1.f, 0.f, 0.f};
    TVECT v2 = {0.f, 1.f, 0.f};
    TVECT v3 = {5.f, 0.f, 0.f};
    if (isEqual(scalarMultVector(v1, v2), 0.0f)) pass++; // Ортогональні
    if (isEqual(scalarMultVector(v1, v3), 5.0f)) pass++; // Колінеарні
    if (isEqual(scalarMultVector(v1, v1), 1.0f)) pass++; // На себе
    return pass == 3;
}

int test_suite_vectorMultVector() {
    int pass = 0;
    TVECT vX = {1.f, 0.f, 0.f};
    TVECT vY = {0.f, 1.f, 0.f};
    TVECT vZ = {0.f, 0.f, 1.f};
    if (isEqual2(vectorMultVector(vX, vY), vZ)) pass++; // X x Y = Z
    if (isEqual2(vectorMultVector(vY, vX), multVector(-1.0f, vZ))) pass++; // Y x X = -Z
    if (isEqual2(vectorMultVector(vX, vX), (TVECT){0,0,0})) pass++; // На себе
    return pass == 3;
}

//
// === 2. БАЗОВА ГЕОМЕТРІЯ ===
//

int test_suite_area_polygon() {
    int pass = 0;
    // 1. Прямокутний трикутник
    TPoint v1[] = {{0.f, 0.f}, {3.f, 0.f}, {0.f, 4.f}};
    Polygone p1 = {3, v1};
    if (isEqual(area_polygon(&p1), 6.0f)) pass++;

    // 2. Квадрат
    TPoint v2[] = {{0.f, 0.f}, {4.f, 0.f}, {4.f, 4.f}, {0.f, 4.f}};
    Polygone p2 = {4, v2};
    if (isEqual(area_polygon(&p2), 16.0f)) pass++;

    // 3. Лінія (площа 0)
    TPoint v3[] = {{0.f, 0.f}, {1.f, 1.f}, {2.f, 2.f}};
    Polygone p3 = {3, v3};
    if (isEqual(area_polygon(&p3), 0.0f)) pass++;
    return pass == 3;
}

int test_suite_inPolygon() {
    int pass = 0;
    TPoint v[] = {{0.f, 0.f}, {4.f, 0.f}, {4.f, 4.f}, {0.f, 4.f}};
    Polygone p = {4, v};

    // 1. Точка всередині
    if (pointsPolygoneInside(&p, (TPoint){2.f, 2.f}) == TRUE) pass++;
    // 2. Точка на межі
    if (pointsPolygoneInside(&p, (TPoint){0.f, 0.f}) == TRUE) pass++;
    // 3. Точка ззовні
    if (pointsPolygoneInside(&p, (TPoint){-1.f, -2.f}) == FALSE) pass++;
    return pass == 3;
}

int test_suite_isConvexPolygone() {
    int pass = 0;
    // 1. Опуклий квадрат
    TPoint v1[] = {{0.f, 0.f}, {4.f, 0.f}, {4.f, 4.f}, {0.f, 4.f}};
    Polygone p1 = {4, v1};
    if (isConvexPolygone(&p1) == TRUE) pass++;

    // 2. Неопукла стрілка
    TPoint v2[] = {{0.f, 0.f}, {4.f, 0.f}, {2.f, 1.f}, {4.f, 4.f}, {0.f, 4.f}};
    Polygone p2 = {5, v2};
    if (isConvexPolygone(&p2) == FALSE) pass++;

    // 3. Трикутник (завжди опуклий)
    TPoint v3[] = {{0.f, 0.f}, {4.f, 0.f}, {2.f, 3.f}};
    Polygone p3 = {3, v3};
    if (isConvexPolygone(&p3) == TRUE) pass++;

    // 4. Лінія (невалідний)
    TPoint v4[] = {{0.f, 0.f}, {4.f, 0.f}};
    Polygone p4 = {2, v4};
    if (isConvexPolygone(&p4) == FALSE) pass++;
    return pass == 4;
}

int test_suite_isIntersectPolygones() {
    int pass = 0;
    Polygone p1 = {.n = 4, .vertice = (TPoint[]){{0,0}, {2,0}, {2,2}, {0,2}}};

    // 1. Явний перетин
    Polygone p_intersect = {.n = 4, .vertice = (TPoint[]){{1,1}, {3,1}, {3,3}, {1,3}}};
    if (isIntersectPolygones(&p1, &p_intersect) == TRUE) pass++;

    // 2. Без перетину
    Polygone p_no_intersect = {.n = 4, .vertice = (TPoint[]){{10,10}, {12,10}, {12,12}, {10,12}}};
    if (isIntersectPolygones(&p1, &p_no_intersect) == FALSE) pass++;

    // 3. Один всередині іншого
    Polygone p_inside = {.n = 4, .vertice = (TPoint[]){{0.5,0.5}, {1.5,0.5}, {1.5,1.5}, {0.5,1.5}}};
    if (isIntersectPolygones(&p1, &p_inside) == TRUE) pass++;

    return pass == 3;
}

int test_suite_convex_wrapper() {
    int pass = 0;
    // 1. Оболонка для неопуклої стрілки (має бути опуклий 4-кутник)
    TPoint v_arrow[] = {{0,0}, {4,0}, {2,1}, {4,4}, {0,4}};
    Polygone p_arrow = {5, v_arrow};
    TPoint v_hull_expected[] = {{0,0}, {4,0}, {4,4}, {0,4}};
    Polygone p_hull_expected = {4, v_hull_expected};

    Polygone hull1 = convex_wrapper(&p_arrow);
    if (isEqualPolygone(&hull1, &p_hull_expected)) pass++;
    freePolygone(&hull1);

    // 2. Оболонка для трикутника (має бути той самий трикутник)
    TPoint v_tri[] = {{0,0}, {3,0}, {0,4}};
    Polygone p_tri = {3, v_tri};
    Polygone hull2 = convex_wrapper(&p_tri);
    if (isEqualPolygone(&hull2, &p_tri)) pass++;
    freePolygone(&hull2);

    // 3. Оболонка для набору точок (складний випадок)
    TPoint v_pts[] = {{0,3}, {1,1}, {2,2}, {4,4}, {0,0}, {1,2}, {3,1}, {3,3}};
    Polygone p_pts = {8, v_pts};
    TPoint v_hull3_exp[] = {{0,0}, {3,1}, {4,4}, {0,3}};
    Polygone p_hull3_exp = {4, v_hull3_exp};

    Polygone hull3 = convex_wrapper(&p_pts);
    if (isEqualPolygone(&hull3, &p_hull3_exp)) pass++;
    freePolygone(&hull3);

    return pass == 3;
}


//
// === 3. АНАЛІЗ ФАЙЛІВ ===
//

// Допоміжна функція для створення тестових файлів
// Створює бінарний файл з 3-ма багатокутниками:
// 0: Квадрат (4 вершини), Периметр = 4, Площа = 1
// 1: Трикутник (3 вершини), Периметр = 12, Площа = 6
// 2: Неопукла стрілка (5 вершин), Периметр ~7.2, Площа ~4.5
void create_test_file(const char* filename) {
    FILE* fp = fopen(filename, "wb");
    assert(fp != NULL);

    unsigned int M = 3;
    fwrite(&M, sizeof(unsigned int), 1, fp);

    // 0: Квадрат P=4, S=1
    Polygone p1 = {.n = 4, .vertice = (TPoint[]){{0,0}, {1,0}, {1,1}, {0,1}}};
    writePolygone_binary(fp, &p1);

    // 1: Трикутник P=12, S=6
    Polygone p_tri = {.n = 3, .vertice = (TPoint[]){{0,0}, {3,0}, {0,4}}};
    writePolygone_binary(fp, &p_tri);

    // 2: Неопукла стрілка P~7.2, S=4.5
    Polygone p3 = {.n = 5, .vertice = (TPoint[]){{2,2}, {4,2}, {3,3}, {4,4}, {2,4}}};
    writePolygone_binary(fp, &p3);

    fclose(fp);
}

int test_suite_isEqualPolygone() {
    int pass = 0;
    // 1. Тест на рівність
    Polygone p1 = {.n = 3, .vertice = (TPoint[]){{0,0}, {1,0}, {0,1}}};
    Polygone p2 = {.n = 3, .vertice = (TPoint[]){{0,0}, {1,0}, {0,1}}};
    if (isEqualPolygone(&p1, &p2) == TRUE) pass++;

    // 2. Тест на різну кількість вершин
    Polygone p3 = {.n = 4, .vertice = (TPoint[]){{0,0}, {1,0}, {1,1}, {0,1}}};
    if (isEqualPolygone(&p1, &p3) == FALSE) pass++;

    // 3. Тест на різні координати
    Polygone p4 = {.n = 3, .vertice = (TPoint[]){{0,0}, {1,0}, {0,2}}};
    if (isEqualPolygone(&p1, &p4) == FALSE) pass++;

    return pass == 3;
}

int test_suite_isPresentPolygone() {
    const char* F = "test_e.bin";
    create_test_file(F);
    FILE* fp = fopen(F, "rb");
    assert(fp != NULL);
    int pass = 0;

    // 1. Пошук першого (квадрат)
    Polygone p1 = {.n = 4, .vertice = (TPoint[]){{0,0}, {1,0}, {1,1}, {0,1}}};
    if (isPresentPolygone(fp, &p1) == TRUE) pass++;

    // 2. Пошук останнього (стрілка)
    Polygone p3 = {.n = 5, .vertice = (TPoint[]){{2,2}, {4,2}, {3,3}, {4,4}, {2,4}}};
    if (isPresentPolygone(fp, &p3) == TRUE) pass++;

    // 3. Пошук неіснуючого
    Polygone p_no = {.n = 3, .vertice = (TPoint[]){{9,9}, {9,8}, {8,9}}};
    if (isPresentPolygone(fp, &p_no) == FALSE) pass++;

    fclose(fp);
    remove(F);
    return pass == 3;
}

int test_suite_deletePolygonesFile() {
    const char* F = "test_d.bin";
    int pass = 0;
    unsigned int M;

    // 1. Видалення першого (індекс 0)
    create_test_file(F);
    deletePolygonesFile(F, 0);
    FILE* fp = fopen(F, "rb");
    fread(&M, sizeof(unsigned int), 1, fp);
    if (M == 2) pass++; // Кількість має зменшитись
    fclose(fp);

    // 2. Видалення середнього (індекс 1)
    create_test_file(F);
    deletePolygonesFile(F, 1);
    fp = fopen(F, "rb");
    fread(&M, sizeof(unsigned int), 1, fp);
    if (M == 2) pass++;
    fclose(fp);

    // 3. Видалення останнього (індекс 2)
    create_test_file(F);
    deletePolygonesFile(F, 2);
    fp = fopen(F, "rb");
    fread(&M, sizeof(unsigned int), 1, fp);
    if (M == 2) pass++;
    fclose(fp);

    remove(F);
    return pass == 3;
}

int test_suite_maxPerimeterPolygone() {
    const char* F = "test_ep.bin";
    create_test_file(F);
    FILE* fp = fopen(F, "rb");
    assert(fp != NULL);
    int pass = 0;
    Polygone p_max = {0, NULL};

    // 1. Тест на коректний пошук
    // У нашому файлі макс. периметр у трикутника P=12
    Polygone p_tri = {.n = 3, .vertice = (TPoint[]){{0,0}, {3,0}, {0,4}}};
    maxPerimeterPolygone(fp, &p_max);
    if (isEqualPolygone(&p_max, &p_tri) == TRUE) pass++;
    freePolygone(&p_max); // Очистка

    // 2. Тест на порожній файл
    fclose(fp);
    fp = fopen(F, "wb"); unsigned int M = 0; fwrite(&M, sizeof(unsigned int), 1, fp); fclose(fp);
    fp = fopen(F, "rb");
    if (maxPerimeterPolygone(fp, &p_max) == FALSE) pass++;

    // 3. Тест на файл з одного елемента
    fclose(fp);
    fp = fopen(F, "wb"); M = 1; fwrite(&M, sizeof(unsigned int), 1, fp);
    Polygone p1 = {.n = 4, .vertice = (TPoint[]){{0,0}, {1,0}, {1,1}, {0,1}}};
    writePolygone_binary(fp, &p1); fclose(fp);

    fp = fopen(F, "rb");
    maxPerimeterPolygone(fp, &p_max);
    if (isEqualPolygone(&p_max, &p1) == TRUE) pass++;

    fclose(fp);
    remove(F);
    freePolygone(&p_max); // Очистка
    return pass == 3;
}

int test_suite_minAreaPolygone() {
    const char* F = "test_min_a.bin";
    create_test_file(F);
    FILE* fp = fopen(F, "rb");
    assert(fp != NULL);
    int pass = 0;
    Polygone p_min = {0, NULL};

    // 1. Тест на коректний пошук
    // У нашому файлі мін. площа у квадрата S=1
    Polygone p_square = {.n = 4, .vertice = (TPoint[]){{0,0}, {1,0}, {1,1}, {0,1}}};
    minAreaPolygone(fp, &p_min);
    if (isEqualPolygone(&p_min, &p_square)) pass++;
    freePolygone(&p_min);

    // 2. Тест на порожній файл
    fclose(fp);
    fp = fopen(F, "wb"); unsigned int M = 0; fwrite(&M, sizeof(unsigned int), 1, fp); fclose(fp);
    fp = fopen(F, "rb");
    if (minAreaPolygone(fp, &p_min) == FALSE) pass++;

    // 3. Тест на файл з одного елемента
    fclose(fp);
    fp = fopen(F, "wb"); M = 1; fwrite(&M, sizeof(unsigned int), 1, fp);
    Polygone p_tri = {.n = 3, .vertice = (TPoint[]){{0,0}, {3,0}, {0,4}}};
    writePolygone_binary(fp, &p_tri); fclose(fp);

    fp = fopen(F, "rb");
    minAreaPolygone(fp, &p_min);
    if (isEqualPolygone(&p_min, &p_tri)) pass++;

    fclose(fp);
    remove(F);
    freePolygone(&p_min);
    return pass == 3;
}

int test_suite_numberConvexPolygones() {
    const char* F = "test_num_c.bin";
    create_test_file(F);
    FILE* fp = fopen(F, "rb");
    assert(fp != NULL);
    int pass = 0;

    // 1. У нашому файлі 2 опуклі фігури (квадрат і трикутник)
    if (numberConvexPolygones(fp) == 2) pass++;

    // 2. Порожній файл
    fclose(fp);
    fp = fopen(F, "wb"); unsigned int M = 0; fwrite(&M, sizeof(unsigned int), 1, fp); fclose(fp);
    fp = fopen(F, "rb");
    if (numberConvexPolygones(fp) == 0) pass++;

    // 3. Файл лише з неопуклих
    fclose(fp);
    fp = fopen(F, "wb"); M = 1; fwrite(&M, sizeof(unsigned int), 1, fp);
    Polygone p_arrow = {.n = 5, .vertice = (TPoint[]){{2,2}, {4,2}, {3,3}, {4,4}, {2,4}}};
    writePolygone_binary(fp, &p_arrow); fclose(fp);
    fp = fopen(F, "rb");
    if (numberConvexPolygones(fp) == 0) pass++;

    fclose(fp);
    remove(F);
    return pass == 3;
}

int test_suite_pointsPolygones() {
    const char* F = "test_points.bin";
    create_test_file(F);
    FILE* fp = fopen(F, "rb");
    assert(fp != NULL);
    int pass = 0;

    // 1. Точка всередині 1 фігури (квадрат)
    if (pointsPolygones(fp, (TPoint){0.5f, 0.5f}) == 1) pass++;

    // 2. Точка всередині 2 фігур (стрілка і трикутник з P=12) - НІ, стрілка в іншому місці
    // Точка всередині 1 фігури (стрілка)
    rewind(fp);
    if (pointsPolygones(fp, (TPoint){3.0f, 3.5f}) == 1) pass++;

    // 3. Точка ззовні (0 фігур)
    rewind(fp);
    if (pointsPolygones(fp, (TPoint){10.0f, 10.0f}) == 0) pass++;

    fclose(fp);
    remove(F);
    return pass == 3;
}


//
// === 4. ПРЕДИКАТИ ===
//

int test_suite_conditionPolygones() {
    const char* F_IN = "test_y.bin";
    const char* F_OUT = "test_y_out.bin";
    create_test_file(F_IN);
    FILE* fp = fopen(F_IN, "rb");
    assert(fp != NULL);
    int pass = 0;
    NTYPE count;

    // 1. Фільтрація опуклих (має бути 2: квадрат і трикутник)
    count = conditionPolygones(fp, &isConvexPolygone, F_OUT);
    if (count == 2) pass++;

    // 2. Фільтрація трикутників (має бути 1)
    rewind(fp); // Повертаємо вказівник файлу
    count = conditionPolygones(fp, &isTrianglePolygone, F_OUT);
    if (count == 1) pass++;

    // 3. Фільтрація чотирикутників (має бути 1)
    rewind(fp);
    count = conditionPolygones(fp, &isQuadrilateralPolygone, F_OUT);
    if (count == 1) pass++;

    fclose(fp);
    remove(F_IN);
    remove(F_OUT);
    return pass == 3;
}

//
// === 5. ЛІНІЇ ТА ПРОМЕНІ ===
//

int test_suite_length_point_ray() {
    TRay R = {{0,0}, {1,0}}; // Промінь з (0,0) вправо вздовж осі X
    int pass = 0;

    // 1. Точка на промені
    if (isEqual(length_point_ray((TPoint){5,0}, R), 0.0f)) pass++;
    // 2. Точка "позаду" променя
    if (isEqual(length_point_ray((TPoint){-5,0}, R), 5.0f)) pass++;
    // 3. Точка збоку
    if (isEqual(length_point_ray((TPoint){2,3}, R), 3.0f)) pass++;

    return pass == 3;
}

int test_suite_length_segments() {
    int pass = 0;
    // 1. Перетин (відстань 0)
    TSegment s1 = {{0,0}, {2,2}};
    TSegment s2 = {{0,2}, {2,0}};
    if (isEqual(length_segments(s1, s2), 0.0f)) pass++;

    // 2. Паралельні
    TSegment s3 = {{0,0}, {2,0}};
    TSegment s4 = {{0,1}, {2,1}};
    if (isEqual(length_segments(s3, s4), 1.0f)) pass++;

    // 3. "Г-подібні"
    TSegment s5 = {{0,0}, {2,0}};
    TSegment s6 = {{0,1}, {0,3}};
    if (isEqual(length_segments(s5, s6), 1.0f)) pass++;

    return pass == 3;
}

int test_suite_linePolygones() {
    const char* F = "test_k.bin";
    create_test_file(F);
    FILE* fp = fopen(F, "rb");
    assert(fp != NULL);
    int pass = 0;

    // 1. Лінія, що перетинає 2 фігури (квадрат і трикутник)
    TLine L1 = {0, 1, -0.5}; // y = 0.5
    if (linePolygones(fp, L1) == 2) pass++;

    // 2. Лінія, що перетинає 0 фігур
    rewind(fp);
    TLine L2 = {0, 1, -10}; // y = 10
    if (linePolygones(fp, L2) == 0) pass++;

    // 3. Лінія, що перетинає 1 фігуру (стрілка)
    rewind(fp);
    TLine L3 = {1, 0, -3}; // x = 3
    if (linePolygones(fp, L3) == 1) pass++;

    fclose(fp);
    remove(F);
    return pass == 3;
}

int test_suite_rayPolygones() {
    const char* F = "test_k_ray.bin";
    create_test_file(F); // Використовуємо ту ж допоміжну функцію
    FILE* fp = fopen(F, "rb");
    assert(fp != NULL);
    int pass = 0;

    // 1. Промінь, що перетинає 1 фігуру (квадрат)
    TRay R1 = {{0.5f, -1.0f}, {0.5f, 1.0f}}; // Вертикальний промінь
    if (rayPolygones(fp, R1) == 1) pass++;

    // 2. Промінь, що не перетинає жодної
    rewind(fp);
    TRay R2 = {{10.0f, 10.0f}, {11.0f, 11.0f}};
    if (rayPolygones(fp, R2) == 0) pass++;

    // 3. Промінь, що перетинає 2 фігури (стрілку і трикутник)
    rewind(fp);
    TRay R3 = {{1.0f, 3.0f}, {4.0f, 3.0f}}; // Горизонтальний
    if (rayPolygones(fp, R3) == 2) pass++;

    fclose(fp);
    remove(F);
    return pass == 3;
}

//
// === 6. ВВІД/ВИВІД ===
//

int test_showPolygoneFile_suite() {
    // Автоматичне тестування виводу на консоль є складним.
    // Ми просто перевіримо, що функція коректно обробляє
    // 3 ситуації, не викликаючи збій.

    const char* F = "test_g.bin";
    create_test_file(F);
    FILE* fp = fopen(F, "rb");
    assert(fp != NULL);

    printf("\n    [ RUN ] showPolygoneFile(0). Очікуємо Квадрат:\n    ");
    showPolygoneFile(fp, 0);

    printf("\n    [ RUN ] showPolygoneFile(2). Очікуємо Стрілку:\n    ");
    showPolygoneFile(fp, 2);

    printf("\n    [ RUN ] showPolygoneFile(99). Очікуємо Помилку індексу:\n    ");
    showPolygoneFile(fp, 99); // Неіснуючий індекс

    fclose(fp);
    remove(F);
    // Цей тест завжди повертає TRUE, якщо він не "впав".
    // Головне - візуально перевірити вивід.
    return TRUE;
}