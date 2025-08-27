#include <iostream>

using namespace std;

void swaping(int* &a, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size - 1; j++) {
			if (a[j] > a[j + 1])
				swap(a[j], a[j + 1]);
		}
	}
}

int main() {
	setlocale(0, "");

	int size;
	cout << "Введите размер массива: "; cin >> size;
	
	char* mas = new char[size * 3];

	srand(time(NULL));
	int counter = 0;
	for (int i = 0; i < size; i++) {
		int digit = rand() % 10;
		*(mas + counter++) = digit + '0';
		if (digit > 0 && rand() % 2 == 0) {
			int secDigit = rand() % 10;
			*(mas + counter++) = '0' + secDigit;
			*(mas + counter++) = ' ';
		}
		else {
			*(mas + counter++) = ' ';
		}
	}
	*(mas + counter) = '\0';
	cout << "Массив чисел(символов): " << mas << endl;

	int* arr = new int[size];

	cout << endl;

	int k, l = 0, contr = 0;

	for (k = 0; k < counter; k++) {
		if (mas[k] != ' ') {
			++contr;
			continue;
		}
		else {
			if (contr == 2) {
				*(arr + l++) = ((*(mas + (k - contr)) - '0') * 10) + (*(mas + (k - contr + 1)) - '0');
				contr = 0;
			}
			else {
				*(arr + l++) = *(mas + (k - contr)) - '0';
				contr = 0;
			}
		}
	}
	//  delete[] mas;  /* с ним, когда в массиве все числа двузначные,
	//  выдает ошибку (нарушение кучи: прога пишет значение после конца буфера кучи)*/
	swaping(arr, size);

	cout << "Отсортированный новый массив: ";
	for (int i = 0; i < size; i++) {
		cout << *(arr + i) << " ";
	}
	cout << endl << endl;
	delete[] arr;
}