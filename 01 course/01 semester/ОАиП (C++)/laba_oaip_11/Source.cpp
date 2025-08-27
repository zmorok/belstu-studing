#include <iostream> 
using namespace std;
int main()
{
	setlocale(LC_CTYPE, "Russian");
	int A = 186; // 10111010
	int B = 167; // 10100111
	char tmp[33];
	int maskA = 14;
	int maskB = ~maskA >> 1;
	cout << "������ ����� �=" << A << endl;
	cout << "������ ����� �=" << B << endl;

	_itoa_s(A, tmp, 2);
	cout << "A=" << tmp << endl;
	// ����� ����� � � �������� �����

	_itoa_s(B, tmp, 2);
	cout << "B=" << tmp << endl;
	// ����� ����� � � �������� �����

	_itoa_s(maskA, tmp, 2);
	cout << "����� ��� �: " << tmp << endl;
	// ����� ����� ��� ����� � � �������� �����, ��� ���������� �����

	_itoa_s((A & maskA) >> 1, tmp, 2);
	cout << "���������� ���� �: " << tmp << endl;
	// ��������� � ��� ����� � � ��� �����, ����� ����� ������ �� 1 ��� ��� �������� ����

	_itoa_s(maskB, tmp, 2);
	cout << "����� ��� �: " << tmp << endl;
	//

	_itoa_s(B & maskB, tmp, 2);
	cout << " ������� ���� � B: " << tmp << endl;

	_itoa_s(((B & maskB) | ((A & maskA)  >> 1)), tmp, 2);
	cout << " ��������� B=" << tmp << endl;



	/*
		3 ���� ����� A ������� �� ������� �� ����� ������ = ****101*;
		����� B � ����� ������ ����� � ������� � ������� �� ����� ������ = 10000011;
	*/
}

