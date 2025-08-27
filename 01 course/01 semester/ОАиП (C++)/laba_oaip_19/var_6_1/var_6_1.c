#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    setlocale(0, "");

    FILE* fileF;
    double userNumber;
    double currentNumber;
    double closestNumber;
    int lineNumber = 0;
    int closestLineNumber = -1;
    double minDifference = INFINITY; 

    fileF = fopen("fA.txt", "r");
    if (fileF == NULL) {
        printf("Не удалось открыть файл fA.txt.\n");
        return 1;
    }

    printf("Введите целое число: ");
    scanf("%i", &userNumber);

    // Считываем числа из файла и находим ближайшее
    while (fscanf(fileF, "%i", &currentNumber) == 1) {
        lineNumber++;

        // Вычисляем разницу между текущим числом и введенным пользователем
        double difference = fabs(userNumber - currentNumber);

        // Если разница меньше текущей минимальной разницы, обновляем значения
        if (difference < minDifference) {
            minDifference = difference;
            closestNumber = currentNumber;
            closestLineNumber = lineNumber;
        }
    }

    fclose(fileF);


    if (closestLineNumber != -1) {
        printf("Число %lf наиболее близко к введенному пользователем числу. Порядковый номер: %d\n", closestNumber, closestLineNumber);
    }
    else {
        printf("Файл fA.txt пуст.\n");
    }

    return 0;
}
