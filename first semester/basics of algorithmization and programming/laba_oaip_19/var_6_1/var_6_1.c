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
        printf("�� ������� ������� ���� fA.txt.\n");
        return 1;
    }

    printf("������� ����� �����: ");
    scanf("%i", &userNumber);

    // ��������� ����� �� ����� � ������� ���������
    while (fscanf(fileF, "%i", &currentNumber) == 1) {
        lineNumber++;

        // ��������� ������� ����� ������� ������ � ��������� �������������
        double difference = fabs(userNumber - currentNumber);

        // ���� ������� ������ ������� ����������� �������, ��������� ��������
        if (difference < minDifference) {
            minDifference = difference;
            closestNumber = currentNumber;
            closestLineNumber = lineNumber;
        }
    }

    fclose(fileF);


    if (closestLineNumber != -1) {
        printf("����� %lf �������� ������ � ���������� ������������� �����. ���������� �����: %d\n", closestNumber, closestLineNumber);
    }
    else {
        printf("���� fA.txt ����.\n");
    }

    return 0;
}
