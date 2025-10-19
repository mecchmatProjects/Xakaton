#include <stdio.h>
#include <assert.h>
#include "Polygone.h"

// Допоміжна функція для створення тестового бінарного файлу
void create_test_file(const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) return;

    // Загальна кількість багатокутників у файлі
    unsigned int M = 3;
    fwrite(&M, sizeof(unsigned int), 1, fp);

    // Багатокутник 1: Опуклий квадрат
    Polygone p1;
    p1.n = 4;
    p1.vertice = (TPoint*)malloc(p1.n * sizeof(TPoint));
    p1.vertice[0] = (TPoint){0.0, 0.0};
    p1.vertice[1] = (TPoint){1.0, 0.0};
    p1.vertice[2] = (TPoint){1.0, 1.0};
    p1.vertice[3] = (TPoint){0.0, 1.0};
    writePolygone_binary(fp, &p1);
    freePolygone(&p1);

    // Багатокутник 2: Неопукла "стрілка"
    Polygone p2;
    p2.n = 5;
    p2.vertice = (TPoint*)malloc(p2.n * sizeof(TPoint));
    p2.vertice[0] = (TPoint){2.0, 2.0};
    p2.vertice[1] = (TPoint){3.0, 2.0};
    p2.vertice[2] = (TPoint){2.5, 3.0}; // "Втиснута" вершина
    p2.vertice[3] = (TPoint){2.0, 4.0};
    p2.vertice[4] = (TPoint){1.5, 3.0};
    writePolygone_binary(fp, &p2);
    freePolygone(&p2);

    // Багатокутник 3: Опуклий трикутник
    Polygone p3;
    p3.n = 3;
    p3.vertice = (TPoint*)malloc(p3.n * sizeof(TPoint));
    p3.vertice[0] = (TPoint){5.0, 0.0};
    p3.vertice[1] = (TPoint){6.0, 2.0};
    p3.vertice[2] = (TPoint){4.0, 2.0};
    writePolygone_binary(fp, &p3);
    freePolygone(&p3);

    fclose(fp);
}

// === ТЕСТ 1: Пошук існуючого багатокутника ===
void test_find_existing_polygon() {
    printf("Test 1: Finding a existent polygon... ");
    const char* filename = "test_data.bin";
    create_test_file(filename);

    FILE* fp = fopen(filename, "rb");
    assert(fp != NULL);

    // Створюємо точну копію квадрата з файлу
    Polygone square_to_find;
    square_to_find.n = 4;
    square_to_find.vertice = (TPoint*)malloc(square_to_find.n * sizeof(TPoint));
    square_to_find.vertice[0] = (TPoint){0.0, 0.0};
    square_to_find.vertice[1] = (TPoint){1.0, 0.0};
    square_to_find.vertice[2] = (TPoint){1.0, 1.0};
    square_to_find.vertice[3] = (TPoint){0.0, 1.0};

    // Перевіряємо, чи функція його знаходить
    int result = isPresentPolygone(fp, &square_to_find);
    assert(result == TRUE); // Має знайти

    printf("PASSED\n");

    freePolygone(&square_to_find);
    fclose(fp);
    remove(filename); // Прибираємо за собою
}

// === ТЕСТ 2: Пошук неіснуючого багатокутника ===
void test_find_non_existing_polygon() {
    printf("Test 2: Finding a non-existent polygon... ");
    const char* filename = "test_data.bin";
    create_test_file(filename);

    FILE* fp = fopen(filename, "rb");
    assert(fp != NULL);

    // Створюємо багатокутник, якого точно немає у файлі
    Polygone random_pentagon;
    random_pentagon.n = 5;
    random_pentagon.vertice = (TPoint*)malloc(random_pentagon.n * sizeof(TPoint));
    for (int i=0; i<5; ++i) random_pentagon.vertice[i] = (TPoint){(PTYPE)i*10, (PTYPE)i*10};

    // Перевіряємо, що функція його НЕ знаходить
    int result = isPresentPolygone(fp, &random_pentagon);
    assert(result == FALSE); // Не має знайти

    printf("PASSED\n");

    freePolygone(&random_pentagon);
    fclose(fp);
    remove(filename);
}

// === ТЕСТ 3: Фільтрація опуклих багатокутників ===
void test_filter_convex_polygons() {
    printf("Test 3: Filtering convex polygons... ");
    const char* in_filename = "test_data.bin";
    const char* out_filename = "convex_only.bin";
    create_test_file(in_filename);

    FILE* fp_in = fopen(in_filename, "rb");
    assert(fp_in != NULL);

    // У нашому тестовому файлі 2 опуклих багатокутники (квадрат і трикутник)
    NTYPE expected_count = 2;
    NTYPE actual_count = conditionPolygones(fp_in, &isConvexPolygone, out_filename);

    assert(actual_count == expected_count);

    // Додаткова перевірка: читаємо створений файл і перевіряємо кількість у ньому
    FILE* fp_out = fopen(out_filename, "rb");
    assert(fp_out != NULL);
    unsigned int count_in_new_file;
    fread(&count_in_new_file, sizeof(unsigned int), 1, fp_out);
    assert(count_in_new_file == expected_count);

    printf("PASSED\n");

    fclose(fp_in);
    fclose(fp_out);
    remove(in_filename);
    remove(out_filename);
}


int main() {
    printf("=== Running tests for the polygon library ===\n");

    test_find_existing_polygon();
    test_find_non_existing_polygon();
    test_filter_convex_polygons();

    printf("=== All tests passed!!! ===\n"); // Я в це не вірю, але хай буде
    return 0;
}