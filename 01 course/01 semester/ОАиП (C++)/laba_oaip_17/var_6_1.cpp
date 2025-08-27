#include <iostream>
using namespace std;

static int sumOfMinAndMax(int* &mas, int size) {
	if (size <= 0) {
		cerr << "Fatal error!" << endl;
		return 1;
	}

	srand(time(NULL));
	cout << "Массив с " << size << " элементами:\n\n[";
	for (int i = 0; i < size; i++) {
		*(mas + i) = rand() % 2 == 1 ? rand() % 101 : (rand() % 101) * -1;
		cout << " " << *(mas + i) << " ";
	}
	cout << "]" << endl;

	int min = *mas, max = *mas;

	for (int i = 0; i < size; i++) {
		if (min > *(mas + i))
			min = *(mas + i);
		if (max < *(mas + i))
			max = *(mas + i);
	}

	int sum = min + max;
	cout << "Сумма минимального (" << min << ") и максимального (" << max << ") элементов: " << sum << endl;

}

int main() {
	setlocale(0, "");
	int size;
	cout << "Введите размер массива: "; cin >> size;

	int* mas = new int[size];

	sumOfMinAndMax(mas, size);

	delete[] mas;
}