#include <iostream>
using namespace std;

int main() {
	setlocale(LC_CTYPE, "Russian");
	float n;
	int digit, sum = 0;
	cout << "������� ���-�� �����: ";
	cin >> n;
	if (n <= 0) {
		cout << "���-�� ����� ������ ���� ������������� � �� ��������� ����!" << endl;
	}
	else if (n - int(n) > 0) {
		cout << "���-�� ����� ������ ���� �����, � �� �������" << endl;
	}
	else {
		for (int i = 1; i < n + 1; i++) {
			cout << "������� �����: ";
			cin >> digit;
			if (i % 2 == 0) {
				sum += digit;
			}
			else { continue; }
		}
		cout << "����� ������ ���������= " << sum << endl;
	}
}