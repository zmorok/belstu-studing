#include <iostream>
#include <locale.h>

int main() {
	setlocale(LC_CTYPE, "Russian");
	using namespace std;
	float startSum, endSum;
	float finalSum = 0;
	int days = 0;
	cout << "������� ��������� � ������ ����: "; cin >> startSum;
	cout << "������� ��������� � ��������� ����: "; cin >> endSum; cout << endl;
	while (startSum < endSum) {
		startSum += 0.03 * startSum;
		days += 1;
		finalSum += startSum;
	}
	cout << "������� � ��������� ����: " << finalSum << endl;
	cout << "���������� ����, ��������� � ������� ���: " << days << endl;
}