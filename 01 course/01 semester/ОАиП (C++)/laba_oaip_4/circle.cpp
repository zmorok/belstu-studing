#include <iostream>
#include <cmath>
#include <cstdio>
using namespace std;

int main() {
	setlocale(LC_CTYPE, "Russian");
	float x = 0, y = 0, z = 0;
	cout << "\t\tЗадание 1" << endl << endl;
	cout << "Введите символ для круга: " << endl;
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

	cout << "\t\tЗадание 2" << endl << endl;
	cout << "Введите числа X, Y, Z: " << endl;
	cin >> x; cin >> y; cin >> z;
	cout << "Введённые числа:" << endl;
	cout << "X= " << x << " Y= " << y << " Z= " << z << endl << endl;
	x = abs(x); y = abs(y); z = abs(z);
	cout << "Модули чисел:" << endl;
	cout << "X= " << x << " Y= " << y << " Z= " << z << endl << endl;
	cout << "Среднее арифметическое модулей= " << (x + y + z) / 3 << endl;
	cout << "Среднее геометрическое модулей= " << pow(x * y * z, 1 / 3.0) << endl << endl;
}