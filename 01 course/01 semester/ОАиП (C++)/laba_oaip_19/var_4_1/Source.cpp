#include <iostream>
#include <stdio.h>

using namespace std;

int main() {
	FILE* file1, * file2;
	errno_t err1, err2;

	err1 = fopen_s(&file1, "file1.txt", "r");
	err2 = fopen_s(&file2, "file2.txt", "a");

	if (err1 != 0 || err2 != 0) {
		perror("Ошибка открытия файлов");
		return 1;
	}

	int number;

	while (fscanf_s(file1, "%d", &number) == 1) {
		if (number > 0) {
			fprintf_s(file2, "\n%d-------", number);
		}
	}

	fclose(file1);
	fclose(file2);
}