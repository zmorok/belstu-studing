#include <iostream>
#include <algorithm>
using namespace std;

int main() {
	setlocale(0, "");

	int n;
	cout << "Введите размер массива: "; cin >> n;

	int *mas = new int[n];
	
	cout << "Исходный массив чисел: " << endl;
	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		*(mas + i) = rand() % 100;
		cout << *(mas + i) << " ";
	}

	cout << endl << endl;

	// копирование mas в B

	int *B = new int[n];
	int *pB = B;

	for (int i = 0; i < n; i++) {
		*(pB + i) = *(mas + i);
	}
	//********************

	sort(mas, mas + n);
	
	int counter = 0;

	cout << "Отсортированный исходный массив чисел: " << endl;
	for (int i = 0; i < n; i++) {
		cout << *(mas + i) << " ";
		if (*(mas + i) != *(mas + i + 1)) {
			counter++;
		}
	}
	cout << endl << endl << "Кол-во различных чисел: " << counter << endl << endl;

	/*for (int i = 0; i < n; i++) {
		cout << *(pB + i) << " ";
	}*/
}