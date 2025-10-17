#include <math.h>
#include <stdlib.h>

#include "Types.h"      // Визначення структур TPoint, TSegment, TLine
#include "vectors.h"    // Функції для роботи з векторами (setVector, lengthVector, etc.)
#include "Predicat.h"   // Функція isEqual для коректного порівняння float
#include "LineRays.h"   // Оголошення функцій цього файлу


TPoint* intersect_lines(TLine l1, TLine l2) {
    // Розв'язуємо систему лінійних рівнянь методом Крамера:
    // a1*x + b1*y = -c1
    // a2*x + b2*y = -c2
    PTYPE det = l1.a * l2.b - l2.a * l1.b;

    // Якщо визначник близький до нуля, прямі паралельні або збігаються.
    if (isEqual(det, 0.0f)) {
        return NULL;
    }

    TPoint* intersection_point = (TPoint*)malloc(sizeof(TPoint));
    if (!intersection_point) {
        // Помилка виділення пам'яті
        return NULL;
    }

    // Знаходимо координати точки за формулами Крамера.
    intersection_point->x = (l2.c * l1.b - l1.c * l2.b) / det;
    intersection_point->y = (l1.c * l2.a - l2.c * l1.a) / det;

    return intersection_point;
}

TPoint* intersect_segments(TSegment s1, TSegment s2) {
    // Представимо відрізки як параметричні рівняння:
    // s1(t) = s1.pointA + t * v1
    // s2(u) = s2.pointA + u * v2
    // де v1 і v2 - напрямні вектори, а 0 <= t, u <= 1.
    TPoint p1 = s1.pointA;
    TPoint p2 = s2.pointA;
    TVECT v1 = setVector(s1.pointA, s1.pointB);
    TVECT v2 = setVector(s2.pointA, s2.pointB);

    PTYPE det = v1.x * (-v2.y) - (-v2.x) * v1.y;

    if (isEqual(det, 0.0f)) {
        // Відрізки паралельні або колінеарні.
        return NULL;
    }


    // Знаходимо параметри t та u
    PTYPE t = ((p2.x - p1.x) * (-v2.y) - (-v2.x) * (p2.y - p1.y)) / det;
    PTYPE u = (v1.x * (p2.y - p1.y) - v1.y * (p2.x - p1.x)) / det;

    // Якщо t та u знаходяться в діапазоні [0, 1], то точка перетину лежить на обох відрізках.
    if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f) {
        TPoint* intersection_point = (TPoint*)malloc(sizeof(TPoint));
        if (!intersection_point) {
            return NULL;
        }

        intersection_point->x = p1.x + t * v1.x;
        intersection_point->y = p1.y + t * v1.y;
        return intersection_point;
    }

    // Інакше точка перетину прямих лежить поза межами хоча б одного з відрізків.
    return NULL;
}

PTYPE length_point_segment(TPoint p, TSegment s) {
    TPoint A = s.pointA;
    TPoint B = s.pointB;

    TVECT v_AB = setVector(A, B); // Вектор відрізка
    TVECT v_AP = setVector(A, p); // Вектор від початку відрізка до точки

    // Квадрат довжини відрізка. Використовуємо його, щоб уникнути зайвого sqrt().
    PTYPE len_sq = scalarMultVector(v_AB, v_AB);

    if (isEqual(len_sq, 0.0f)) {
        // Якщо відрізок - це точка (A і B збігаються).
        return lengthVector(v_AP);
    }

    // Знаходимо проекцію точки P на нескінченну пряму, що містить відрізок AB.
    // t - це параметр, що показує, де знаходиться проекція.
    PTYPE dot_product = scalarMultVector(v_AP, v_AB);
    PTYPE t = dot_product / len_sq;

    TPoint closest_point;
    if (t < 0.0) {
        // Проекція знаходиться "лівіше" від точки А. Найближча точка на відрізку - це А.
        closest_point = A;
    } else if (t > 1.0) {
        // Проекція знаходиться "правіше" від точки В. Найближча точка - це В.
        closest_point = B;
    } else {
        // Проекція знаходиться на відрізку AB.
        closest_point.x = A.x + t * v_AB.x;
        closest_point.y = A.y + t * v_AB.y;
    }

    // Повертаємо відстань від точки p до знайденої найближчої точки.
    TVECT dist_vec = setVector(p, closest_point);
    return lengthVector(dist_vec);
}


PTYPE length_point_line(TPoint p, TLine line) {
    return fabs(line.a * p.x + line.b * p.y + line.c) / sqrt(line.a * line.a + line.b * line.b);
}

TPoint* intersect_rays(TRay r1, TRay r2) {
    // Промені - це частини прямих. Спочатку знайдемо перетин прямих.
    // r1: p1 + t * v1 (t >= 0)
    // r2: p2 + u * v2 (u >= 0)
    TPoint p1 = r1.start_point;
    TPoint p2 = r2.start_point;
    TVECT v1 = setVector(r1.start_point, r1.direction_point);
    TVECT v2 = setVector(r2.start_point, r2.direction_point);

    PTYPE det = v1.x * (-v2.y) - (-v2.x) * v1.y;

    if (isEqual(det, 0.0f)) {
        return NULL; // Паралельні
    }

    PTYPE t = ((p2.x - p1.x) * (-v2.y) - (-v2.x) * (p2.y - p1.y)) / det;
    PTYPE u = (v1.x * (p2.y - p1.y) - v1.y * (p2.x - p1.x)) / det;

    // Головна відмінність від відрізків: параметри t та u мають бути НЕВІД'ЄМНИМИ.
    if (t >= 0.0f && u >= 0.0f) {
        TPoint* intersection_point = (TPoint*)malloc(sizeof(TPoint));
        if (!intersection_point) return NULL;
        intersection_point->x = p1.x + t * v1.x;
        intersection_point->y = p1.y + t * v1.y;
        return intersection_point;
    }

    return NULL; // Перетин прямих є, але він поза межами одного з променів.
}