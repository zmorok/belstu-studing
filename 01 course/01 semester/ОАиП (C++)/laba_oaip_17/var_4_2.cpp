#include <iostream>
using namespace std;

static int findingAndChangingRow(int mas[][4], bool flag, int rowWithPositive) {
	cout << "Изначальный массив:" << endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << "[ " << *(*(mas + i) + j) << " ]";
		}
		cout << endl;
	}

	for (int i = 0; i < 4; i++) {
		if (flag) {
			for (int j = 0; j < 4; j++) {
				if (*(*(mas + i) + j) > 0) {
					flag = false;
					rowWithPositive = i;
				}
			}
		}
	}

	if (flag) {
		cout << endl << "В матрице все элементы отрицательны." << endl;
		return 0;
	}
	else if (rowWithPositive == 0) {
		for (int j = 0; j < 4; j++) {
			*(*(mas + 3) + j) *= -1;
		}
		cout << endl << "Положительное число в первой строке -> У последней поменялся знак." << endl << endl;
	}
	else {
		for (int j = 0; j < 4; j++) {
			*(*(mas + (rowWithPositive - 1)) + j) *= -1;
		}
		cout << endl << "Номер строки с первым положительным элементом: " << rowWithPositive + 1 << endl << endl;
	}

	cout << "Конечный массив:" << endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << "[ " << *(*(mas + i) + j) << " ]";
		}
		cout << endl;
	}
	return 0;
}

int main() {
	setlocale(0, "");

	int mas[4][4] = {
		{-1,2,-3,-4},
		{-5,-6,-7,-8},
		{-9,-10,-11,-12},
		{-13,-14,-15,16}
	};

	bool flag = true;
	int rowWithPositive = -1;

	findingAndChangingRow(mas, flag, rowWithPositive);
}