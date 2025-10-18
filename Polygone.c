
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "Types.h"
#include "vectors.h"
#include "Polygone.h"
#include "file_forming.h"
#include "LineRays.h"

#define LEN 30

int freePolygone(Polygone* p){
    if(p->vertice) free(p->vertice);
    return 0;
}

int inputPolygone(FILE* fp, Polygone* p){
    NTYPE n;
    int is_console = (fp == NULL);

    if (is_console) {
        printf("Введіть кількість вершин N = ");
        fscanf_s(stdin, "%u", &n);
    } else {
        fscanf_s(fp, "%u", &n);
    }

    if (n < 3) {
        p->n = 0;
        p->vertice = NULL;
        return FALSE;
    }

    p->n = n;
    p->vertice = (TPoint*) malloc(n * sizeof(TPoint));
    if (!p->vertice) return FALSE;

    for (NTYPE i = 0; i < n; ++i) {
        int scan_res = 0;
        if (is_console) {
            printf("Вершина %u (x y): ", i + 1);
            scan_res = fscanf_s(stdin, "%f %f", &p->vertice[i].x, &p->vertice[i].y);
        } else {
            scan_res = fscanf_s(fp, "%f %f", &p->vertice[i].x, &p->vertice[i].y);
        }
        if (scan_res != 2) {
            free(p->vertice);
            p->vertice = NULL;
            p->n = 0;
            return FALSE;
        }
    }

    return TRUE;
}

int writePolygone_binary(FILE* fp, Polygone* p) {
    assert(fp != NULL && p != NULL);
    fwrite(&p->n, sizeof(NTYPE), 1, fp);
    fwrite(&p->vertice, sizeof(TPoint), p->n, fp);
    return 1;
}

int writePolygone_text(FILE* fp, Polygone* p) {
    assert(fp != NULL && p != NULL);
    fprintf(fp, "%u ", p->n);
    for (NTYPE i = 0; i < p->n; i++) {
        fprintf(fp, "%.2f %.2f ", p->vertice[i].x, p->vertice[i].y);
    }
    fprintf(fp, "\n");
    return 1;
}

void showPolygonesFile(FILE* fp) {
    Polygone* polygones = readPolygones(fp);
    if (!polygones) return;

    int i = 0;
    while(polygones[i].n != 0) {
        outputPolygon(polygones[i]);
        freePolygone(&polygones[i]);
        i++;
    }
    free(polygones);
}

int deletePolygonesFile(const char* fname, NTYPE k) {
    FILE* fp = fopen(fname, "rb");
    if (!fp) return FALSE;

    unsigned int M;
    if (fread(&M, sizeof(unsigned int), 1, fp) != 1) {
        fclose(fp);
        return FALSE;
    }

    if (k >= M) {
        fclose(fp);
        return FALSE;
    }

    Polygone* arr = (Polygone*)malloc(M * sizeof(Polygone));
    if (!arr) {
        fclose(fp);
        return FALSE;
    }

    for (unsigned i = 0; i < M; ++i) {
        fread(&arr[i].n, sizeof(NTYPE), 1, fp);
        arr[i].vertice = (TPoint*)malloc(arr[i].n * sizeof(TPoint));
        fread(arr[i].vertice, sizeof(TPoint), arr[i].n, fp);
    }
    fclose(fp);

    fp = fopen(fname, "wb");
    if (!fp) {
        for (unsigned i = 0; i < M; ++i) free(arr[i].vertice);
        free(arr);
        return FALSE;
    }
    
    unsigned NM = M - 1;
    fwrite(&NM, sizeof(unsigned), 1, fp);

    for (unsigned i = 0; i < M; ++i) {
        if (i != k) {
            writePolygone_binary(fp, &arr[i]);
        }
        free(arr[i].vertice);
    }
    
    free(arr);
    fclose(fp);
    return TRUE;
}

PTYPE area_polygon(const Polygone* p) {
    if (p->n < 3) return 0.0f;

    PTYPE area = 0.0f;
    for (NTYPE i = 0; i < p->n; ++i) {
        TPoint p1 = p->vertice[i];
        TPoint p2 = p->vertice[(i + 1) % p->n];
        area += (p1.x * p2.y - p2.x * p1.y);
    }
    return fabsf(area) / 2.0f;
}

PTYPE perimeterPolygone(const Polygone* p) {
    if (!p || p->n < 2) return 0.0f;
    PTYPE perimeter = 0.0f;
    for (NTYPE i = 0; i < p->n; ++i) {
        TPoint p1 = p->vertice[i];
        TPoint p2 = p->vertice[(i + 1) % p->n];
        TVECT edge_vector = setVector(p1, p2);
        perimeter += lengthVector(edge_vector);
    }
    return perimeter;
}

int isConvexPolygone(const Polygone* p) {
    if (p->n < 3) return FALSE;

    int sign = 0;
    for (NTYPE i = 0; i < p->n; ++i) {
        TPoint p1 = p->vertice[i];
        TPoint p2 = p->vertice[(i + 1) % p->n];
        TPoint p3 = p->vertice[(i + 2) % p->n];

        TVECT v1 = setVector(p1, p2);
        TVECT v2 = setVector(p2, p3);

        PTYPE cross_product_z = v1.x * v2.y - v1.y * v2.x;

        if (!isEqual(cross_product_z, 0.0f)) {
            int current_sign = (cross_product_z > 0) ? 1 : -1;
            if (sign == 0) {
                sign = current_sign;
            } else if (sign != current_sign) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

int maxPerimeterPolygone(FILE* fp, Polygone* out) {
    if (!fp || !out) return FALSE;

    rewind(fp);
    unsigned M = 0;
    if (fscanf_s(fp, "%u", &M) != 1 || M == 0) return FALSE;

    Polygone best = {0, NULL};
    PTYPE bestPer = -1.0f;

    for (unsigned i = 0; i < M; ++i) {
        Polygone cur = {0, NULL};
        if (fscanf_s(fp, "%u", &cur.n) != 1 || cur.n < 3) {
            return FALSE;
        }
        cur.vertice = (TPoint*)calloc(cur.n, sizeof(TPoint));
        if (!cur.vertice) return FALSE;

        for (unsigned j=0; j<cur.n; ++j) {
            if (fscanf_s(fp, "%f %f", &cur.vertice[j].x, &cur.vertice[j].y) != 2) {
                free(cur.vertice);
                return FALSE;
            }
        }

        PTYPE per = perimeterPolygone(&cur);
        if (per > bestPer) {
            if (best.vertice) free(best.vertice);
            best = cur;      // забираємо володіння пам'яттю
            bestPer = per;
        } else {
            free(cur.vertice);
        }
    }

    *out = best;
    return TRUE;
}

int minAreaPolygone(FILE* fp, Polygone* p_min) {
    assert(fp != NULL);
    rewind(fp);

    unsigned int M;
    if (fread(&M, sizeof(unsigned int), 1, fp) != 1) return FALSE;

    PTYPE min_area = -1.0f;
    int found = FALSE;

    for (unsigned int i = 0; i < M; i++) {
        Polygone temp_p;
        fread(&temp_p.n, sizeof(NTYPE), 1, fp);
        temp_p.vertice = (TPoint*)malloc(temp_p.n * sizeof(TPoint));
        fread(temp_p.vertice, sizeof(TPoint), temp_p.n, fp);

        PTYPE area = area_polygon(&temp_p);
        if (!found || area < min_area) {
            min_area = area;
            found = TRUE;
            if (p_min->vertice) free(p_min->vertice);
            p_min->n = temp_p.n;
            p_min->vertice = (TPoint*)malloc(temp_p.n * sizeof(TPoint));
            memcpy(p_min->vertice, temp_p.vertice, temp_p.n * sizeof(TPoint));
        }
        freePolygone(&temp_p);
    }
    return found;
}

NTYPE numberConvexPolygones(FILE* fp) {
    assert(fp != NULL);
    rewind(fp);

    unsigned int M;
    if (fread(&M, sizeof(unsigned int), 1, fp) != 1) return 0;

    NTYPE count = 0;
    for (unsigned int i = 0; i < M; i++) {
        Polygone p;
        fread(&p.n, sizeof(NTYPE), 1, fp);
        p.vertice = (TPoint*)malloc(p.n * sizeof(TPoint));
        fread(p.vertice, sizeof(TPoint), p.n, fp);

        if (isConvexPolygone(&p)) {
            count++;
        }
        freePolygone(&p);
    }
    return count;
}


NTYPE pointsPolygones(FILE* fp, TPoint point) {
    assert(fp != NULL);
    rewind(fp);
    unsigned int M;
    if (fread(&M, sizeof(unsigned int), 1, fp) != 1) return 0;

    NTYPE res = 0;
    for (unsigned int i = 0; i < M; i++) {
        Polygone p;
        fread(&p.n, sizeof(unsigned int), 1, fp);
        p.vertice = (TPoint*) malloc(p.n*sizeof(TPoint));
        fread(p.vertice, sizeof(TPoint), p.n, fp);

        if (pointsPolygoneInside(&p, point)) {
            res++;
        }
        freePolygone(&p);
    }
    return res;
}

int pointsPolygoneInside(const Polygone* p, TPoint p0) {
    if (!p || p->n < 3) return FALSE;

    int intersections = 0;
    for (NTYPE i = 0; i < p->n; ++i) {
        TPoint p1 = p->vertice[i];
        TPoint p2 = p->vertice[(i + 1) % p->n];

        TSegment s = {p1, p2};
        if (isEqual(length_point_segment(p0, s), 0.0f)) return TRUE;

        if (((p1.y > p0.y) != (p2.y > p0.y)) &&
            (p0.x < (p2.x - p1.x) * (p0.y - p1.y) / (p2.y - p1.y) + p1.x)) {
            intersections++;
        }
    }
    return (intersections % 2) == 1;
}

int isTrianglePolygone(const Polygone* p) {
    return (p && p->n == 3);
}

int isQuadrilateralPolygone(const Polygone* p) {
    return (p && p->n == 4);
}

int isIntersectPolygones(const Polygone* p1, const Polygone* p2) {
    if (!p1 || !p2) return FALSE;

    for (NTYPE i = 0; i < p1->n; ++i) {
        TSegment s1 = {p1->vertice[i], p1->vertice[(i + 1) % p1->n]};
        for (NTYPE j = 0; j < p2->n; ++j) {
            TSegment s2 = {p2->vertice[j], p2->vertice[(j + 1) % p2->n]};
            TPoint* intersection = intersect_segments(s1, s2);
            if (intersection) {
                free(intersection);
                return TRUE;
            }
        }
    }

    if (p1->n > 0 && pointsPolygoneInside(p2, p1->vertice[0])) return TRUE;
    if (p2->n > 0 && pointsPolygoneInside(p1, p2->vertice[0])) return TRUE;

    return FALSE;
}

static int comparePoints(const void* a, const void* b) {
    TPoint* p1 = (TPoint*)a; TPoint* p2 = (TPoint*)b;
    if (!isEqual(p1->x, p2->x)) return (p1->x < p2->x) ? -1 : 1;
    return (p1->y < p2->y) ? -1 : 1;
}
static PTYPE cross_product(TPoint a, TPoint b, TPoint c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

Polygone convex_wrapper(const Polygone* p1) {
    Polygone hull = {0, NULL};
    if (!p1 || p1->n < 3) return hull;

    TPoint* points = (TPoint*)malloc(p1->n * sizeof(TPoint));
    memcpy(points, p1->vertice, p1->n * sizeof(TPoint));
    qsort(points, p1->n, sizeof(TPoint), comparePoints);

    TPoint* temp_hull = (TPoint*)malloc(2 * p1->n * sizeof(TPoint));
    int k = 0;
    for (NTYPE i = 0; i < p1->n; ++i) {
        while (k >= 2 && cross_product(temp_hull[k - 2], temp_hull[k - 1], points[i]) <= 0) k--;
        temp_hull[k++] = points[i];
    }
    for (int i = p1->n - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && cross_product(temp_hull[k - 2], temp_hull[k - 1], points[i]) <= 0) k--;
        temp_hull[k++] = points[i];
    }

    hull.n = k - 1;
    hull.vertice = (TPoint*)malloc(hull.n * sizeof(TPoint));
    memcpy(hull.vertice, temp_hull, hull.n * sizeof(TPoint));

    free(points);
    free(temp_hull);
    return hull;
}

// ЗАГЛУШКИ
Polygone intersect_polygone(const Polygone* p1, const Polygone* p2) {
    printf("Warning: intersect_polygone is not implemented.\n");
    return (Polygone){0, NULL};
}
NTYPE triangulatePolygone(const Polygone* p1, TTriangle* tr) {
    printf("Warning: triangulatePolygone is not implemented.\n");
    return 0;
}
Polygone wrapper_distance(const Polygone* p1, PTYPE dist) {
    printf("Warning: wrapper_distance is not implemented.\n");
    return (Polygone){0, NULL};
}

/*
============================task-e=======================================
*/

int isEqualPolygone(const Polygone* p1, const Polygone* p2) {
    assert(p1 != NULL);
    assert(p2 != NULL);

    // 1. Перевіряємо, чи збігається кількість вершин
    if (p1->n != p2->n) {
        return FALSE;
    }

    // 2. Послідовно перевіряємо координати кожної вершини
    for (NTYPE i = 0; i < p1->n; i++) {
        // Використовуємо isEqual для коректного порівняння чисел з плаваючою комою
        if (!isEqual(p1->vertice[i].x, p2->vertice[i].x) || !isEqual(p1->vertice[i].y, p2->vertice[i].y)) {
            return FALSE; // Знайдено першу невідповідність
        }
    }

    // Якщо всі вершини збіглися, багатокутники рівні
    return TRUE;
}

int isPresentPolygone(FILE* fp, const Polygone* p) {
    assert(fp != NULL);
    assert(p != NULL);

    // Читаємо загальну кількість багатокутників у файлі
    unsigned int M;
    if (fread(&M, sizeof(unsigned int), 1, fp) != 1) {
        return FALSE; // Помилка читання або порожній файл
    }

    Polygone temp_p;
    temp_p.vertice = NULL;
    int found = FALSE;

    for (unsigned int i = 0; i < M; i++) {
        // Читаємо наступний багатокутник з файлу
        fread(&temp_p.n, sizeof(unsigned int), 1, fp);
        temp_p.vertice = (TPoint*)malloc(temp_p.n * sizeof(TPoint));
        if (temp_p.vertice == NULL) {
            // Не вдалося виділити пам'ять, перериваємо пошук
            break;
        }

        for (int j = 0; j < temp_p.n; j++) {
            fread(&temp_p.vertice[j].x, sizeof(PTYPE), 1, fp);
            fread(&temp_p.vertice[j].y, sizeof(PTYPE), 1, fp);
        }

        // Порівнюємо прочитаний багатокутник із тим, що шукаємо
        if (isEqualPolygone(&temp_p, p)) {
            found = TRUE;
            free(temp_p.vertice); // Звільняємо пам'ять
            break; // Знайшли, можна завершувати цикл
        }

        // Якщо не знайшли, просто звільняємо пам'ять і продовжуємо
        free(temp_p.vertice);
        temp_p.vertice = NULL;
    }

    // Повертаємо курсор файлу в початкове положення, щоб не впливати
    // на подальші операції з файлом (добра практика)
    fseek(fp, 0, SEEK_SET);

    return found;
}