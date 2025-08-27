#include <iostream>
using namespace std;

// насколько битов (1) больше битов (0)
int main() {
	setlocale(LC_ALL, "Russian");
	int A;
	cout << "Введите число: "; cin >> A;

	int mask = 1;
	char tmp[33];
	int countOfOnes = 0, countOfZeros = 0;

	_itoa_s(A, tmp, 2);
	cout << "Число " << A << " в двоичной форме: " << tmp << endl << endl;
	
	int size = 0;
	for (int i = 0; tmp[i] != '\0'; i++) {
		size += 1;
	}

	for (int i = 0; i < size; i++) {
		if (A & mask) {
			countOfOnes++;
		}
		else {
			countOfZeros++;
		}
		mask <<= 1;
	}

	cout << "Единички: " << countOfOnes << ", Нули: " << countOfZeros << endl;
	cout << "В числе '" << A << "' битов с 1 больше, чем битов с 0 на '" << countOfOnes - countOfZeros << "'" << endl;
}