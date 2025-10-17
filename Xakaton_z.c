#include "Polygone.h"

// ------------------ Перевірка опуклості ------------------
int isConvexPolygone(const Polygon *p) {
    int n = p->n;
    if (n < 3) return 0;      // менше трьох вершин не багатокутник
    int t = 0;                // напрямок повороту
    int i;
    for (i = 0; i < n; i++) {
        Point a = p->v[i];
        Point b = p->v[(i+1)%n];
        Point c = p->v[(i+2)%n];
        float cross = (b.x - a.x)*(c.y - b.y) - (b.y - a.y)*(c.x - b.x);
        if (cross < 1e-6 && cross > -1e-6) continue; // майже на одній прямій
        if (t == 0)
            t = (cross > 0) ? 1 : -1;
        else if ((cross > 0 && t < 0) || (cross < 0 && t > 0))
            return 0;          // напрямок змінився → не опуклий
    }
    return 1;                  // всі кути в один бік → опуклий
}

// ------------------ Підрахунок опуклих багатокутників ------------------
int numberConvexPolygones(FILE *fp) {
    if (!fp) return -1;        // перевірка на NULL
    int M, i, j, count = 0;
    if (fscanf(fp, "%d", &M) != 1) return -1; // читаємо кількість багатокутників
    for (i = 0; i < M; i++) {
        Polygon poly;
        if (fscanf(fp, "%d", &poly.n) != 1) return -1;
        for (j = 0; j < poly.n; j++) {
            if (fscanf(fp, "%f %f", &poly.v[j].x, &poly.v[j].y) != 2) return -1;
        }
        if (isConvexPolygone(&poly))
            count++;             // збільшуємо лічильник
    }
    return count;
}
