#include <iostream>
using namespace std;

int main() {
	setlocale(0, "");
	
	int n;
	cout << "Введите размер массива: "; cin >> n;

	int* mas = new int[n];

	int sum = 0;

	cout << "\nИсходный массив чисел: " << endl;
	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		*(mas + i) = rand() % 100;
		cout << *(mas + i) << " ";
		sum += *(mas + i);
	}

	
	int sredni = sum / n;
	cout << "\nСреднее арифм. суммы элементов: " << sredni << endl << endl;
	
	int size1 = 0, size2 = 0;

	for (int i = 0; i < n; i++) {
		if (*(mas + i) > sredni) {
			size1++;
		}
		else {
			size2++;
		}
	}


	int *overSredni = new int[size1];
	int *underSredni = new int[size2];
	
	int ind1 = 0, ind2 = 0;

	for (int i = 0; i < n; i++) {
		if (*(mas + i) > sredni) {
			*(overSredni + ind1++) = *(mas + i);
		}
		else {
			*(underSredni + ind2++) = *(mas + i);
		}
	}

	cout << "Массив чисел, которые больше среднего арифм.: " << endl;
	for (ind1 = 0; ind1 < size1; ind1++) {
		cout << *(overSredni + ind1) << " ";
	}
	cout << endl;

	cout << "Массив чисел, которые меньше среднего арифм.: " << endl;
	for (ind2 = 0; ind2 < size2; ind2++) {
		cout << *(underSredni + ind2) << " ";
	}
	cout << endl;

	delete[] mas;
	delete[] overSredni;
	delete[] underSredni;
}