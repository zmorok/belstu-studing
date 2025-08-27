#include <iostream>
#include <locale.h>

using namespace std;

int greatestCommonDivisor(int a, int b) {
	int result = min(a, b);
	while (result > 0) {
		if (a % result == 0 && b % result == 0) {
			cout << "��� ��� ����� " << a << " � " << b << " = " << result << endl;
			break;
		}
		result--;
	}
	return result;
}
int main() {
	int a, b;
	setlocale(LC_CTYPE, "Russian");
	cout << "������� ��� ����������� �����: ";
	cin >> a >> b;
	if (a <= 0 || b <= 0) {
		cout << "����, ������������� � ������� ����� �� �������� ������������." << endl;
	} 
	else {
		greatestCommonDivisor(a, b);
	}
}