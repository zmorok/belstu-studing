#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main() {
    setlocale(0, "");
    FILE* fileA, * fileB, * fileC;
    int rowsA, colsA, rowsB, colsB;

    fileA = fopen("A.txt", "r");
    fileB = fopen("B.txt", "r");

    if (fileA == NULL || fileB == NULL) {
        printf("Не удалось открыть файлы A.txt или B.txt.\n");
        return 1;
    }

    fscanf(fileA, "%d %d", &rowsA, &colsA);
    fscanf(fileB, "%d %d", &rowsB, &colsB);

    if (colsA != rowsB) {
        printf("Невозможно выполнить умножение матриц: количество столбцов матрицы A не равно количеству строк матрицы B.\n");
        fclose(fileA);
        fclose(fileB);
        return 1;
    }

    fileC = fopen("C.txt", "w");
    if (fileC == NULL) {
        printf("Не удалось открыть файл C.txt для записи.\n");
        fclose(fileA);
        fclose(fileB);
        return 1;
    }

    // Выделяем память под матрицы A, B и C
    int** matrixA = (int**)malloc(rowsA * sizeof(int*));
    int** matrixB = (int**)malloc(rowsB * sizeof(int*));
    int** matrixC = (int**)malloc(rowsA * sizeof(int*));

    if (matrixA == NULL || matrixB == NULL || matrixC == NULL) {
        printf("Не удалось выделить память для матриц.\n");
        fclose(fileA);
        fclose(fileB);
        fclose(fileC);
        return 1;
    }

    for (int i = 0; i < rowsA; i++) {
        matrixA[i] = (int*)malloc(colsA * sizeof(int));
        matrixC[i] = (int*)malloc(colsB * sizeof(int));
    }

    for (int i = 0; i < rowsB; i++) {
        matrixB[i] = (int*)malloc(colsB * sizeof(int));
    }

    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsA; j++) {
            fscanf(fileA, "%d", &matrixA[i][j]);
        }
    }

    for (int i = 0; i < rowsB; i++) {
        for (int j = 0; j < colsB; j++) {
            fscanf(fileB, "%d", &matrixB[i][j]);
        }
    }

    // Умножаем матрицы и записываем результат в файл C
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            matrixC[i][j] = 0;
            for (int k = 0; k < colsA; k++) {
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
            fprintf(fileC, "%d ", matrixC[i][j]);
        }
        fprintf(fileC, "\n");
    }

    for (int i = 0; i < rowsA; i++) {
        free(matrixA[i]);
        free(matrixC[i]);
    }

    for (int i = 0; i < rowsB; i++) {
        free(matrixB[i]);
    }

    free(matrixA);
    free(matrixB);
    free(matrixC);

    fclose(fileA);
    fclose(fileB);
    fclose(fileC);

    printf("Процесс завершен успешно.\n");

    return 0;
}
