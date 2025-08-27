#include <iomanip>
#include <iostream>
#include <conio.h>
using namespace std;

int main() {
	setlocale(LC_CTYPE, "Russian");	
	char c;
	cout << "Введите символ "; cin >> c; 
	for (int i = 15; i > 0; i--) {
		cout << setw(i) << setfill(c) << c << endl;
	}
	double x = 1234.56789;
	cout << setw(6) << setprecision(2) << x << endl;

	cout << "Нажмите ПРОБЕЛ для закрытия окна" << endl;
	_getch();
}