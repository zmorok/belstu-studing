#include <iostream>
#include <math.h>
#include <locale.h>
int main() {
	setlocale(LC_CTYPE, "Russian");
	using namespace std;
	int k, l, m, n;
	cout << "������� ���������� �����:" << endl;
	cin >> k >> l;
	cout << "������� ���������� ������:" << endl;
	cin >> m >> n;
	if ((k >= 1 && k <= 9) && (l >= 1 && l <= 9) && (m >= 1 && m <= 9) && (n >= 1 && n <= 9)) {
		if (abs(k - m) == abs(l - n)) {
			cout << "���� �������� ������." << endl;
		}
		else {
			cout << "���� �� �������� ������." << endl;
		}	
	}
	else {
		cout << "�� ����� �������� ���������� �����. ������� �������� �� 1 �� 9" << endl;
	}
}