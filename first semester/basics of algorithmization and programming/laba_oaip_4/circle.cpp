#include <iostream>
#include <cmath>
#include <cstdio>
using namespace std;

int main() {
	setlocale(LC_CTYPE, "Russian");
	float x = 0, y = 0, z = 0;
	cout << "\t\t������� 1" << endl << endl;
	cout << "������� ������ ��� �����: " << endl;
	char c = getchar();
	printf("\t\t\t       %c%c \n", c, c);
	printf("\t\t\t     %c%c%c%c%c%c \n", c, c, c, c, c, c);
	printf("\t\t\t   %c%c%c%c%c%c%c%c%c%c\n", c, c, c, c, c, c, c, c, c, c);
	printf("\t\t\t  %c%c%c%c%c%c%c%c%c%c%c%c\n", c, c, c, c, c, c, c, c, c, c, c, c);
	printf("\t\t\t %c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", c, c, c, c, c, c, c, c, c, c, c, c, c, c);
	printf("\t\t\t %c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", c, c, c, c, c, c, c, c, c, c, c, c, c, c);
	printf("\t\t\t  %c%c%c%c%c%c%c%c%c%c%c%c\n", c, c, c, c, c, c, c, c, c, c, c, c);
	printf("\t\t\t   %c%c%c%c%c%c%c%c%c%c\n", c, c, c, c, c, c, c, c, c, c);
	printf("\t\t\t     %c%c%c%c%c%c\n", c, c, c, c, c, c);
	printf("\t\t\t       %c%c \n", c, c);
	cout << endl;

	cout << "\t\t������� 2" << endl << endl;
	cout << "������� ����� X, Y, Z: " << endl;
	cin >> x; cin >> y; cin >> z;
	cout << "�������� �����:" << endl;
	cout << "X= " << x << " Y= " << y << " Z= " << z << endl << endl;
	x = abs(x); y = abs(y); z = abs(z);
	cout << "������ �����:" << endl;
	cout << "X= " << x << " Y= " << y << " Z= " << z << endl << endl;
	cout << "������� �������������� �������= " << (x + y + z) / 3 << endl;
	cout << "������� �������������� �������= " << pow(x * y * z, 1 / 3.0) << endl << endl;
}