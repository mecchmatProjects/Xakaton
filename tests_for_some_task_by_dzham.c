#include <stdio.h>
#include <assert.h>
#include "Polygone.h"

/*
================================================================================
    ДОПОМІЖНІ ФУНКЦІЇ ТА ПРЕДИКАТИ (визначені лише для цього тесту)
================================================================================
*/

/**
 * @brief Створює тестовий бінарний файл з набором різних багатокутників.
 *
 * Файл міститиме:
 * 1. Опуклий квадрат (4 вершини)
 * 2. Неопукла "стрілка" (5 вершин)
 * 3. Опуклий трикутник (3 вершини)
 * 4. Опуклий п'ятикутник (5 вершин)
 * @param filename Ім'я файлу для створення.
 */
void create_mixed_test_file(const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) return;

    unsigned int M = 4;
    fwrite(&M, sizeof(unsigned int), 1, fp);

    Polygone p1 = {.n = 4, .vertice = (TPoint[]){{0,0}, {1,0}, {1,1}, {0,1}}};
    writePolygone_binary(fp, &p1);

    Polygone p2 = {.n = 5, .vertice = (TPoint[]){{2,2}, {3,2}, {2.5,3}, {2,4}, {1.5,3}}};
    writePolygone_binary(fp, &p2);

    Polygone p3 = {.n = 3, .vertice = (TPoint[]){{5,0}, {6,2}, {4,2}}};
    writePolygone_binary(fp, &p3);

    Polygone p4 = {.n = 5, .vertice = (TPoint[]){{0,5}, {1,4}, {2,4}, {2,5}, {1,6}}};
    writePolygone_binary(fp, &p4);

    fclose(fp);
}

/**
 * @brief Предикат для тестування: перевіряє, чи є багатокутник трикутником.
 * Визначений локально, щоб не змінювати Polygone.c.
 */
int isTriangle_for_test(const Polygone* p) {
    assert(p != NULL);
    return p->n == 3;
}

/**
 * @brief Предикат для тестування: завжди повертає FALSE.
 * Використовується для перевірки випадку, коли немає збігів.
 */
int predicate_always_false_for_test(const Polygone* p) {
    // Цей предикат не залежить від багатокутника
    (void)p;
    return FALSE;
}

/*
================================================================================
                                        ТЕСТИ ДЛЯ ЗАВДАННЯ (й)
================================================================================
*/

// === ТЕСТ 1: Фільтрація за умовою "бути трикутником" ===
void test_filter_triangles() {
    printf("Test (y.1): Filtering under the condition 'is a triangle'...");
    const char* in_file = "mixed_data.bin";
    const char* out_file = "triangles.bin";
    create_mixed_test_file(in_file);

    FILE* fp = fopen(in_file, "rb");
    assert(fp != NULL);

    // У тестовому файлі є лише один трикутник.
    NTYPE expected_count = 1;
    // Передаємо вказівник на нашу локальну тестову функцію isTriangle_for_test
    NTYPE actual_count = conditionPolygones(fp, &isTriangle_for_test, out_file);

    assert(actual_count == expected_count);
    printf("PASSED\n");

    fclose(fp);
    remove(in_file);
    remove(out_file);
}

// === ТЕСТ 2: Фільтрація за умовою "бути опуклим" ===
void test_filter_convex() {
    printf("Test (y.2): Filtering under the condition 'is convex'... ");
    const char* in_file = "mixed_data.bin";
    const char* out_file = "convex.bin";
    create_mixed_test_file(in_file);

    FILE* fp = fopen(in_file, "rb");
    assert(fp != NULL);

    // У файлі 3 опуклі фігури (квадрат, трикутник, п'ятикутник)
    NTYPE expected_count = 3;
    // Використовуємо функцію isConvexPolygone, яка вже є в бібліотеці
    NTYPE actual_count = conditionPolygones(fp, &isConvexPolygone, out_file);

    assert(actual_count == expected_count);
    printf("PASSED\n");

    fclose(fp);
    remove(in_file);
    remove(out_file);
}

// === ТЕСТ 3: Фільтрація з умовою, яка ніколи не виконується ===
void test_filter_no_matches() {
    printf("Test (y.3): Filtering with a condition that does not give matches...");
    const char* in_file = "mixed_data.bin";
    const char* out_file = "empty.bin";
    create_mixed_test_file(in_file);

    FILE* fp = fopen(in_file, "rb");
    assert(fp != NULL);

    // Очікуємо 0 відфільтрованих багатокутників
    NTYPE expected_count = 0;
    // Передаємо вказівник на нашу локальну функцію, що завжди повертає FALSE
    NTYPE actual_count = conditionPolygones(fp, &predicate_always_false_for_test, out_file);

    assert(actual_count == expected_count);
    printf("PASSED\n");

    fclose(fp);
    remove(in_file);
    remove(out_file);
}

/*
================================================================================
                                ТОЧКА ВХОДУ
================================================================================
*/

int main() {
    printf("=== Running tests for a function conditionPolygones (й) [Standalone] ===\n");

    test_filter_triangles();
    test_filter_convex();
    test_filter_no_matches();

    printf("=== All tests passed ===\n"); // ;)
    return 0;
}