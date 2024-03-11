#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <locale.h>

int main() {
    setlocale(0, "");
    FILE* fileF1, * fileF2;
    char line[100]; // ������������, ��� ������ �� ����� ��������� 100 ��������


    fileF1 = fopen("F1.txt", "r");
    fileF2 = fopen("F2.txt", "w");

    if (fileF1 == NULL || fileF2 == NULL) {
        printf("�� ������� ������� ����� F1.txt ��� F2.txt.\n");
        fclose(fileF1);
        fclose(fileF2);
        return 1;
    }

    while (fgets(line, sizeof(line), fileF1) != NULL) {
        if (toupper(line[0]) == 'A' || toupper(line[0] == '�')) {
            fprintf(fileF2, "%s", line);
        }
    }

    fclose(fileF1);
    fclose(fileF2);

    printf("������� �������� �������.\n");

    return 0;
}
