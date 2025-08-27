#include <iostream>
using namespace std;

int main() {
	setlocale(0, "");
	float A[8] = { 1, 2, 3, 4, 5, 6, 13, 13 };
	float B[7] = { 7, 5, 9, 10, 11.5, 12, 13 };

	float maxA = *A;
	for (int i = 0; i < 8; i++) {
		if (maxA < *(A + i)) {
			maxA = *(A + i);
		}
	}
	cout << "Максимальный элемент массива A: " << maxA << endl;

	for (int i = 0; i < 7; i++) {
		if (maxA == *(B + i)) {
			cout << "Наибольший элемент массива A (" << maxA << ") содержится в массиве B" << endl;
			break;
		}
		else { 
			if (i == 6 && maxA != *(B + 6)) {
				cout << "В массиве В нет наибольшего элемента массива А" << endl;
				break;
			}
			else {
				continue;
			}
		}
	}
}
