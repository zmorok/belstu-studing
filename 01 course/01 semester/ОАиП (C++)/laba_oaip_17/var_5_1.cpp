#include <iostream>
using namespace std;

static void sumOfEven(int*& mas, int sum) {
	srand(time(NULL));
	for (int i = 0; i < 15; i++) {
		*(mas + i) = rand() % 2 == 1 ? rand() % 201 : (rand() % 201) * -1;
	}

	cout << "Массив из 15 произвольных элементов: " << endl;
	cout << "[";
	for (int i = 0; i < 15; i++) {
		cout << " " << *(mas + i) << " ";
	}
	cout << "]" << endl << endl;

	for (int i = 0; i < 15; i++) {
		if (*(mas + i) % 2 == 0)
			sum += *(mas + i);
	}

	cout << "Сумма чётных элементов данного массива= " << sum << endl << endl;
}

int main() {
	setlocale(0, "");
	int* mas = new int[15];
	int sum = 0;

	cout << "Адрес: " << mas << endl << endl;

	sumOfEven(mas, sum);

	cout << "Адрес: " << mas << endl << endl;

	delete[] mas;
}