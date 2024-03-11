#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>

int main() {
    setlocale(0, "");
    FILE* fileA, * fileB;
    int number;

    if (!(fileA = fopen("fileA.txt", "r"))) {
        printf("�� ������� ������� ���� fileA.\n");
        return 1;
    }

    if (!(fileB = fopen("fileB.txt", "w"))) {
        printf("�� ������� ������� ���� fileB.\n");
        fclose(fileA);
        return 1;
    }

    while (fscanf(fileA, "%d", &number) == 1) {
        if (number > 0) {
            fprintf(fileB, "%d\n", number);
        }
    }

    fclose(fileA);
    fclose(fileB);

    printf("������� ��������� �������.\n");

    return 0;
}
