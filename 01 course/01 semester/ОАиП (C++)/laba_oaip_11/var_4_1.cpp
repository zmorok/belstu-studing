#include <iostream>
using namespace std;

// ����� ������ 4
int main() {
	setlocale(LC_ALL, "Russian");
	int A;
	cout << "������� ���� �����: "; cin >> A;

	if ((A & 7) == 0) {
		cout << "����� " << A << " ������ �������." << endl;
	}
	else {
		cout << "����� " << A << " �� ������ �������." << endl;
	}
}