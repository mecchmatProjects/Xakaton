#include <stdio.h>
#include <stdlib.h>
#include "Types.h"
#include "Predicat.h"
#include "Polygone.h"
#include "file_forming.h"
#include "testsPolygone.h"

// --- Функції для запуску тестів ---
void run_test(const char* test_name, int (*test_function)()) {
    printf("  [ TEST ] %s... ", test_name);
    if (test_function() == TRUE) {
        printf("\033[0;32m[ PASS ]\033[0m\n");
    } else {
        printf("\033[0;31m[ FAIL ]\033[0m\n");
    }
}

// --- Функція для обробки даних з файлу ---
void process_input_file(const char* filename) {
    printf("--- Обробка файлу '%s' ---\n", filename);
    FILE* fp = fopen(filename, "rb"); // Використовуємо "rb" для бінарного читання, як інші функції
    if (fp == NULL) {
        printf("Помилка: Не вдалося відкрити файл '%s'.\n", filename);
        return;
    }

    // Приклад використання: порахуємо опуклі багатокутники
    NTYPE convex_count = numberConvexPolygones(fp);
    printf("Знайдено опуклих багатокутників: %u\n", convex_count);


    fclose(fp);

    printf("--- Завершено обробку файлу '%s' ---\n\n", filename);
}


// --- Головна функція ---
int main() {

    FILE* input_file_check = fopen("input.dat", "rb"); // Використовуємо "rb", оскільки функції аналізу файлів (minArea, numberConvex...) працюють з бінарним форматом

    if (input_file_check != NULL) {
        // Якщо файл існує, закриваємо перевірочний дескриптор і обробляємо його
        fclose(input_file_check);
        process_input_file("input.dat");

    } else {

        printf("--- Файл 'input.dat' не знайдено. Запускаємо лише тести. ---\n\n");
    }

    // --- Запуск юніт-тестів ---
    printf("--- Запуск тестів для Xakaton 2025 ---\n\n");

    // --- 1. Тести векторної алгебри ---
    printf("--- Модуль: vectors.c ---\n");
    run_test("isEqual / isEqual2", test_suite_isEqual);
    run_test("setVector", test_suite_setVector);
    run_test("lengthVector", test_suite_lengthVector);
    run_test("addVector / subVector", test_suite_add_sub_Vector);
    run_test("multVector", test_suite_multVector);
    run_test("scalarMultVector", test_suite_scalarMultVector);
    run_test("vectorMultVector", test_suite_vectorMultVector);

    // --- 2. Тести базової геометрії ---
    printf("\n--- Модуль: Polygone.c (Базова геометрія) ---\n");
    run_test("area_polygon (Площа)", test_suite_area_polygon);
    run_test("pointsPolygoneInside (Точка всередині)", test_suite_inPolygon);
    run_test("isConvexPolygone (Опуклість)", test_suite_isConvexPolygone);
    run_test("isIntersectPolygones (Перетин)", test_suite_isIntersectPolygones);
    run_test("convex_wrapper (Опукла оболонка)", test_suite_convex_wrapper);

    // --- 3. Тести аналізу файлів ---
    printf("\n--- Модуль: Polygone.c (Аналіз файлів) ---\n");
    run_test("isEqualPolygone (Пункт 'е')", test_suite_isEqualPolygone);
    run_test("isPresentPolygone (Пункт 'е')", test_suite_isPresentPolygone);
    run_test("deletePolygonesFile (Пункт 'д')", test_suite_deletePolygonesFile);
    run_test("maxPerimeterPolygone (Пункт 'є')", test_suite_maxPerimeterPolygone);
    run_test("minAreaPolygone (Пункт 'ж')", test_suite_minAreaPolygone);
    run_test("numberConvexPolygones (Пункт 'з')", test_suite_numberConvexPolygones);
    run_test("pointsPolygones (Пункт 'і')", test_suite_pointsPolygones);

    // --- 4. Тести фільтрації ---
    printf("\n--- Модуль: Predicat.c (Пункт 'й') ---\n");
    run_test("conditionPolygones (Фільтрація)", test_suite_conditionPolygones);

    // --- 5. Тести ліній/променів ---
    printf("\n--- Модуль: linerays.c (Пункт 'к') ---\n");
    run_test("length_point_ray", test_suite_length_point_ray);
    run_test("length_segments", test_suite_length_segments);
    run_test("linePolygones", test_suite_linePolygones);
    run_test("rayPolygones", test_suite_rayPolygones);

    // --- 6. Тести вводу/виводу ---
    printf("\n--- Модуль: file_forming.c & Polygone.c (I/O) ---\n");
    run_test("showPolygoneFile (Пункт 'г')", test_showPolygoneFile_suite);

    printf("\n--- Тестування завершено ---\n");
    return 0;
}