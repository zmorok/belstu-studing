#include <iostream>
#include <locale.h>
int main() {
	setlocale(LC_CTYPE, "Russian");
	using namespace std;
	int a, b, r, s;
	cout << "������� ������� ������� (a x b)" << endl;
	cin >> a >> b;
	cout << "������� ������� ���� (r x s)" << endl;
	cin >> r >> s;
	int areaRegion = a * b;
	int areaHouse = r * s;
	if (areaRegion - (areaHouse * 2) >= 0) {
		cout << "�� ������� ����� ���������� ��� ����." << endl;
	}
	else {
		cout << "�� ������� ������ ���������� ��� ����." << endl;
	}
}