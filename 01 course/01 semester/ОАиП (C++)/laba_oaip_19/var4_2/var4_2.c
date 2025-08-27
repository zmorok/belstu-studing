#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>

int main() {
	setlocale(0, "");

	FILE* f, * g;

	int num;
	printf("Введите ваше двухзначное число: ");
	scanf("%d", &num);

	f = fopen("f.txt", "r");

	if (f == NULL) {
		printf("Не удалось открыть файл f.txt\n");
		return 1;
	}

	g = fopen("g.txt", "w");
	
	if (g == NULL) {
		printf("Не удалось открыть файл g.txt\n");
		fclose(f);
		return 1;
	}


	int number;
	while (fscanf(f, "%d", &number) == 1) {
		if (number > num) {
			fprintf(g, "%d\n", number);
		}
	}

	fclose(f);
	fclose(g);

	printf("Процесс завершёен успешно.\n");

	return 0;
}