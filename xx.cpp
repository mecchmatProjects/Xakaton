#include <cstdio>

void ShowPolygineFile(FILE* fp, int k) {
    if (!fp) {
        printf("помилка: файл не відкрито\n") ;
        return ;
    }
    if (k < 0) {
        printf("Помилка індекс має бути невід'ємним\n");
        return ;
    }
    long start_pos = ftell(fp);

    int polygons_count;
    if (fscanf (fp, "%d", &polygons_count) != 1) {
        printf(" Помилка читання кількості багатокутників\n");
        return ;
    }
    for (int i= 0; i < k; i++) {
        int vertices_count;
        if (fscanf (fp, "%d", &vertices_count) != 1) {
            printf("Помилка\n");
            return ;
        }
        for (int j = 0; j < vertices_count; j++) {
            float x, y;
            if (fscanf (fp, "%f %f", &x, &y) != 2) {
                printf("Error\n");
                return ;
            }
        }
    }
    int vertices_count;
    if (fscanf (fp, "%d", &vertices_count) != 1 || vertices_count < 3 ) {
        printf("Error\n");
        return ;
    }
    printf("Багатокутник %d (%d вершин):", k, vertices_count);
    for (int j = 0; j < vertices_count; j++) {
        float x, y;
        if (fscanf (fp, "%f %f", &x, &y) != 2) {
            printf("Error\n");
            return ;
        }
        printf("(%.2f, %.2f)", x, y);
    }
    printf("\n");
    fseek(fp, start_pos, SEEK_SET);
}
