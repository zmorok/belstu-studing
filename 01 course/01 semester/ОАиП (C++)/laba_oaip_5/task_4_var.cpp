#include <iostream>
#include <locale.h>

int main() {
	setlocale(LC_CTYPE, "Russian");
	using namespace std;
	int N = 0;
	cout << "������� ����� ������: ";
	cin >> N;
	switch (N) {
	case 12: puts("����"); break;
	case 1: puts("����"); break;
	case 2: puts("����"); break;
	case 3: puts("�����"); break;
	case 4: puts("�����"); break;
	case 5: puts("�����"); break;
	case 6: puts("����"); break;
	case 7: puts("����"); break;
	case 8: puts("����"); break;
	case 9: puts("�����"); break;
	case 10: puts("�����"); break;
	case 11: puts("�����"); break;
	default: puts("�� ����� �������� ����� ������"); break;
	}
}