#include <iostream>
#include <locale.h>

int main() {
	setlocale(LC_CTYPE, "Russian");
	using namespace std;
	int a , b , c;
	char s[] = "Error";
	cout << "��� �����: ";
	cin >> a >> b >> c;

	/* int sum = (a % 5 == 0 ? a : 0) + (b % 5 == 0 ? b : 0) + (c % 5 == 0 ? c : 0);
	������ �����, ������� ������� �� 5, ��������� � ����� */

	int sum = ((a + b + c) % 5 == 0 ? (a + b + c) : !((a + b) % 5) ? (a + b) : !((a + c) % 5) ? (a + c) : !((c + b) % 5) ? (c + b) : !(a % 5) ? a : !(b % 5) ? b : !(c % 5) ? c : 0);
	// ��� ��������� �������� ����, ��������� �� 5

	if (sum != 0) {
		cout << "�����, ��������� �� 5: " << sum << endl;
	}
	else {
		cout << "Error" << endl;
	}
}