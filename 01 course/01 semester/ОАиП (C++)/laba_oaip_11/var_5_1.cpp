#include <iostream>
using namespace std;

// ��������� ����� (1) ������ ����� (0)
int main() {
	setlocale(LC_ALL, "Russian");
	int A;
	cout << "������� �����: "; cin >> A;

	int mask = 1;
	char tmp[33];
	int countOfOnes = 0, countOfZeros = 0;

	_itoa_s(A, tmp, 2);
	cout << "����� " << A << " � �������� �����: " << tmp << endl << endl;
	
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

	cout << "��������: " << countOfOnes << ", ����: " << countOfZeros << endl;
	cout << "� ����� '" << A << "' ����� � 1 ������, ��� ����� � 0 �� '" << countOfOnes - countOfZeros << "'" << endl;
}