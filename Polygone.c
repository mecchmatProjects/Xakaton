#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "Types.h"
#include "vectors.h"
#include "Polygone.h"
#include "file_forming.h"

#define LEN 30

int freePolygone(Polygone* p){
    if(p->vertice)free(p->vertice);
    return 0;
}

int inputPolygone(FILE* fp, Polygone* p){

    NTYPE n;
    
    if(fp)
        fscanf(fp,"%u",&n);    
    else{
        printf("N=");
        fscanf(stdin," %u",&n);
    }

    if(n<=2) return FALSE;
    
    p->n = n;
    p->vertice = (TPoint*) malloc(n * 2 * sizeof(TPoint));
    int scan_res = 0;
    // ....
}

int writePolygone_binary(FILE* fp, Polygone* p) {
    assert(fp != 0);
    assert(p != 0);
    int size = fwrite(&p->n, sizeof(NTYPE), 1, fp);
    if (size != 1) return 0;
    for (int i = 0; i < p->n; i++) {
        int size1 = fwrite(&p->vertice[i].x, sizeof(PTYPE), 1, fp);
        int size2 = fwrite(&p->vertice[i].y, sizeof(PTYPE), 1, fp);
        if (size1 != 1 || size2 != 1) return 0;
    }
    return 1;
}

// text file
int writePolygone(FILE* fp, Polygone* p) {
    assert(fp != 0);
    assert(p != 0);
    //
    
}

void showPolygonesFile(FILE* fp) {
    Polygone* polygones = readPolygones(fp);
    int i = 0;
    while(1) {
        if (polygones[i].n == 0) {
            break;
        }
        outputPolygon(polygones[i]); 
        i++;
    }
}

int deletePolygonesFile(FILE* fp, NTYPE k) {
    if (!fp) return FALSE;

    //Зчитуємо всі полігони у пам’ять (масив із сентінелом n==0)
    Polygone* arr = readPolygones(fp);
    if (!arr) return FALSE;

    //Порахувати M за сентінелом
    unsigned M = 0;
    while (arr[M].n != 0) M++;

    //Перевірити індекс
    if (k >= M) {
        for (unsigned i = 0; i < M; ++i) free(arr[i].vertice);
        free(arr);
        return FALSE;
    }

    //Перевідкрити файл у режимі "wb" щоб обнулити та перезаписати
    if (!freopen(NULL, "wb", fp)) {
        for (unsigned i = 0; i < M; ++i) free(arr[i].vertice);
        free(arr);
        return FALSE;
    }

    //Записати нове M і всі полігони, крім видаленого k
    unsigned NM = M - 1;
    if (fwrite(&NM, sizeof(unsigned), 1, fp) != 1) {
        for (unsigned i = 0; i < M; ++i) free(arr[i].vertice);
        free(arr);
        return FALSE;
    }

    for (unsigned i = 0; i < M; ++i) {
        if (i == k) {                    // пропустити видаляємий
            free(arr[i].vertice);
            continue;
        }
        // writePolygone_binary записує: n, потім n пар (x,y)
        if (!writePolygone_binary(fp, &arr[i])) {
            // звільнити решту та впасти
            free(arr[i].vertice);
            for (unsigned j = i + 1; j < M; ++j) free(arr[j].vertice);
            free(arr);
            return FALSE;
        }
        free(arr[i].vertice);
    }

    free(arr);
    return TRUE;
}


PTYPE area(TPoint p1, TPoint p2, TPoint p3) {
    TVECT v1 = setVector(p2, p1);
    TVECT v2 = setVector(p2, p3);
    PTYPE par_area = lengthVector(vectorMultVector(v1, v2));
    return par_area / 2.0;
}


PTYPE area_polygon(Polygone p) {
 
 
}

NTYPE inPolygon(Polygone p, TPoint point) {
    NTYPE power = p.n;
    PTYPE res = 0;
    for (int i = 0; i < power - 1; i++) {
        res += area(p.vertice[i], point, p.vertice[i+1]);
    }
    res += area(p.vertice[0], point, p.vertice[power-1]);
    if (isEqual(area_polygon(p), res)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

NTYPE pointsPolygones(FILE* fp, TPoint point) {
    assert(fp != 0);
    int i = 0, res = 0, n = 0;
    unsigned int M;
    fread(&M, sizeof(unsigned int), 1, fp);
    for (int i = 0; i < M; i++) {
        Polygone p;
        fread(&p.n, sizeof(unsigned int), 1, fp);
        p.vertice = (TPoint*) malloc(p.n*sizeof(TPoint));
        for (int j = 0; j < p.n; j++) {
            fread(&p.vertice[j].x, sizeof(PTYPE), 1, fp);
            fread(&p.vertice[j].y, sizeof(PTYPE), 1, fp);
        }
        if (inPolygon(p, point)) {
            res++;
        }
        free(p.vertice);
    }
    return res;
}

int minAreaPolygone(FILE* fp, Polygone* p) {
    assert(fp != NULL);
    Polygone temp;
    PTYPE min_area = 0;
    int found = FALSE;
    unsigned int M;
    fread(&M, sizeof(unsigned int), 1, fp);
    for (int i = 0; i < M; i++) {
        fread(&temp.n, sizeof(unsigned int), 1, fp);
        temp.vertice = (TPoint*)malloc(temp.n * sizeof(TPoint));
        for (int j = 0; j < temp.n; j++) {
            fread(&temp.vertice[j].x, sizeof(PTYPE), 1, fp);
            fread(&temp.vertice[j].y, sizeof(PTYPE), 1, fp);
        }
        PTYPE area = area_polygon(temp);
        if (!found || area < min_area) {
            min_area = area;
            found = TRUE;
            if (p->vertice) {
                free(p->vertice);
            }
            p->n = temp.n;
            p->vertice = (TPoint*)malloc(temp.n * sizeof(TPoint));
            for (int j = 0; j < temp.n; j++){
                p->vertice[j].x = temp.vertice[j].x;
                p->vertice[j].y = temp.vertice[j].y;
            }
        }
        free(temp.vertice);
    }
    return found;
}

int isConvexPolygone(const Polygone* p) {
    if (p->n < 3) return FALSE;
    int sign = 0;
    for (int i = 0; i < p->n; i++) {
        TPoint p1 = p->vertice[i];
        TPoint p2 = p->vertice[(i + 1) % p->n];
        TPoint p3 = p->vertice[(i + 2) % p->n];
        TVECT v1 = setVector(p1, p2);
        TVECT v2 = setVector(p2, p3);
        /// Векторний добуток

        if (cross_product_z != 0) {
            int current_sign = (cross_product_z > 0) ? 1 : -1;
            if (sign == 0){
                sign = current_sign;
            }else if (sign != current_sign){
                return FALSE;
            }
        }
    }
    return TRUE;
}

NTYPE numberConvexPolygones(FILE* fp) {
    assert(fp != NULL);
    unsigned int M;
    fread(&M, sizeof(unsigned int), 1, fp);
    int count = 0;
    for (int i = 0; i < M; i++) {
        Polygone p;
        fread(&p.n, sizeof(unsigned int), 1, fp);
        p.vertice = (TPoint*)malloc(p.n * sizeof(TPoint));
        for (int j = 0; j < p.n; j++) {
            fread(&p.vertice[j].x, sizeof(PTYPE), 1, fp);
            fread(&p.vertice[j].y, sizeof(PTYPE), 1, fp);
        }
        if (isConvexPolygone(&p)) {
            count++;
        }
        free(p.vertice);
    }
    return count;
}
