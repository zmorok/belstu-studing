#include <iostream>
#include <locale.h>
#include <new>

int main() {
	setlocale(LC_ALL, "Russian");
	using namespace std;
	int  i, k;
	float size;

	cout << "Введите размер массива: ";
	cin >> size;

	if (size < 0) {
		cout << "Ошибка";
		return 1;
	}
	else if ((int)size - size != 0) {
		cout << "Ошибка";
		return 1;
	}

	cout << "Массив с размером в " << size << " элементов:" << endl;
	int* mas = new int[size];
	srand((unsigned)time(NULL));
	for (i = 0; i < size; i++) {
		mas[i] = rand() % 100;
		cout << mas[i] << " ";
	}
	cout << endl << endl;

	cout << "Введите номер элемента (от 1 до " << size << "), который хотите удалить: ";
	cin >> k;
	cout << "Новый массив без " << k << "-го элемента: " << endl;
	if (k > size && k < 0) {
		return 1;
	}
	else {
		for (i = 0; i < size - 1; i++) {
			if (i < k - 1) {
				cout << mas[i] << " ";
			}
			else {
				mas[i] = mas[i + 1];
				cout << mas[i] << " ";
			}
		}
	}
	cout << endl << endl;

	cout << "Массив с нулями после четных элементов: " << endl;
	for (i = 0; i < size - 1; i++) {
		if (mas[i] % 2 == 0) {
			size++; 
			int* newMas = new int[size];
			for (int j = 0; j <= i; j++) {
				newMas[j] = mas[j];
			}

			newMas[i + 1] = 0;

			for (int j = i + 2; j < size; j++) {
				newMas[j] = mas[j - 1];
			}
			delete[] mas; 
			mas = newMas;  
			i++;
		}
	}
	for (i = 0; i < size - 1; i++) {
		cout << mas[i] << " ";
	}
	cout << endl;
	return 0;
}
