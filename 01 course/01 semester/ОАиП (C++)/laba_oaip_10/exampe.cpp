#include <iostream>
#include <locale.h>
#include <time.h>
#include <new>

int main() {
	setlocale(LC_ALL, "Russian");
	using namespace std;
	int n, i, k, size;

	cout << "Введите размер массива: ";
	cin >> size;
	n = size;
	cout << "Массив с размером в " << size << " элементов:" << endl;
	int* mas = new int[size]; // создаем указатель на целое число и выделяем память для массива целых чисел
	srand((unsigned)time(NULL));
	for (i = 0; i < n; i++) {
		mas[i] = rand() % 100;
		cout << mas[i] << " ";
	}
	cout << endl << endl;

	cout << "Введите номер элемента (от 1 до " << size << "), который хотите удалить : ";
	cin >> k;
	cout << "Новый массив без " << k <<"-го элемента: " << endl;
	if (k > size && k < 0) {
		return 1;
	}
	else {
		for (i = 0; i < size-1; i++) {
			if (i < k-1) {
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
			size++; // увеличиваем размер на 1 для добавления нуля
			int* newMas = new int[size];
			for (int j = 0; j <= i; j++) {
				newMas[j] = mas[j];
			}
			
			newMas[i + 1] = 0;

			for (int j = i + 2; j < size; j++) {
				newMas[j] = mas[j-1];
			}
			delete[] mas; // освобождаем память
			mas = newMas; // исходный массив с нулем после четного элемента 
			i++; // пропускаем только что добавленный нуль
		}	
	}
	for (i = 0; i < size-1; i++) {
		cout << mas[i] << " ";
	}
	cout << endl;
}
