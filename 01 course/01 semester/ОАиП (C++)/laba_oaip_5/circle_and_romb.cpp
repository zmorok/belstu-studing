#include <iostream>
#include <locale.h>
#include <math.h>
#include <cmath>

int main() {
	setlocale(LC_CTYPE, "Russian");
	using namespace std;
	int r, p, q;
	cout << "������� ������ (r) ����:" << endl;
	cin >> r;
	cout << "������� ��������� (p, q) �������������� ���������:" << endl;
	cin >> p >> q;
	float side = sqrt((p * p) + (q * q)) / 2;
	float r_1 = ((p * q) / (4 * side)) == r ? true : false;
	if (r_1 == true) {
		cout << "��� ������ ����� ���������" << endl;
	}
	else {
		cout << "��� �� ������ ����� ���������" << endl;
	}
}