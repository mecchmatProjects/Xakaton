#include "Predicat.h"
#include <stdio.h>
#include <assert.h>

int isEqual(PTYPE a, PTYPE b){
    return fabs(a-b) < __DBL_EPSILON__;
}

int isEqual2(TVECT a, TVECT b){
    return isEqual(a.x,b.x) && isEqual(a.y,b.y) && isEqual(a.z, b.z);
}

//My updates (й)
NTYPE conditionPolygones(FILE* fp, predicatPolygone Q, const char* fname) {
    assert(fp != NULL);
    assert(Q != NULL);
    assert(fname != NULL);

    // Відкриваємо вихідний файл для бінарного запису
    FILE* fp_out = fopen(fname, "wb");
    if (fp_out == NULL) {
        perror("Output file creation failed");
        return 0; // Повертаємо 0, якщо не вдалося створити файл
    }

    // Читаємо загальну кількість багатокутників у вхідному файлі
    unsigned int M;
    if (fread(&M, sizeof(unsigned int), 1, fp) != 1) {
        fclose(fp_out);
        return 0; // Помилка читання або порожній файл
    }

    NTYPE written_count = 0;
    Polygone temp_p;
    temp_p.vertice = NULL; // Важлива ініціалізація

    // Спочатку записуємо 0 у вихідний файл як заповнювач для кількості.
    // Пізніше ми повернемося і запишемо сюди реальну кількість.
    fwrite(&written_count, sizeof(NTYPE), 1, fp_out);

    // Проходимо по всіх багатокутниках у вхідному файлі
    for (unsigned int i = 0; i < M; i++) {
        // Читаємо один багатокутник
        fread(&temp_p.n, sizeof(unsigned int), 1, fp);
        temp_p.vertice = (TPoint*)malloc(temp_p.n * sizeof(TPoint));
        if (temp_p.vertice == NULL) { // Перевірка виділення пам'яті
             // Обробка помилки: можливо, варто зупинити виконання
             continue;
        }
        for (int j = 0; j < temp_p.n; j++) {
            fread(&temp_p.vertice[j].x, sizeof(PTYPE), 1, fp);
            fread(&temp_p.vertice[j].y, sizeof(PTYPE), 1, fp);
        }

        // Ключовий момент: викликаємо функцію-предикат Q
        if (Q(&temp_p)) {
            // Якщо предикат повернув TRUE, записуємо багатокутник у вихідний файл
            writePolygone_binary(fp_out, &temp_p);
            written_count++;
        }

        // Звільняємо пам'ять, виділену для поточного багатокутника
        free(temp_p.vertice);
        temp_p.vertice = NULL;
    }

    // Повертаємося на початок вихідного файлу
    fseek(fp_out, 0, SEEK_SET);
    // Записуємо фінальну кількість відфільтрованих багатокутників
    fwrite(&written_count, sizeof(NTYPE), 1, fp_out);

    // Закриваємо вихідний файл
    fclose(fp_out);

    // Повертаємо кількість записаних багатокутників
    return written_count;
}