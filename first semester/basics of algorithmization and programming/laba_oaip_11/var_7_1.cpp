#include <iostream>
using namespace std;

// ���������� 4 ����� ����� A, ������� � ������ �� ����� ������, � ���������� �� � ����� B ������
int main() {
	setlocale(0, "");
	int A, B;
	int maskA = ((1 << 4) - 1) << 4;
	char tmp[33];

	cout << "������� ����� � (>=16): "; cin >> A;
	cout << "������� ����� B: "; cin >> B;

	_itoa_s(maskA, tmp, 2);
	cout << "����� ��� A: " << tmp << endl;

	_itoa_s(A, tmp, 2);
	cout << "����� � � �������� ������� ���������: " << tmp << endl;
	_itoa_s(B, tmp, 2);
	cout << "����� B � �������� ������� ���������: " << tmp << endl;

	_itoa_s((A & maskA) >> 4, tmp, 2);
	cout << "������ 4 ���� ����� �� 5�� ������� ����� A: " << tmp << endl;

	_itoa_s(((A & maskA) >> 4) | (B << 4), tmp, 2);
	cout << "����� B � 4 ������� ������ � ������: " << tmp << endl;
}