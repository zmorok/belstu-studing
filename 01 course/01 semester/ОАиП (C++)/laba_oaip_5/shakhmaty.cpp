#include <iostream>
#include <math.h>
#include <locale.h>
int main() {
	setlocale(LC_CTYPE, "Russian");
	using namespace std;
	int k, l, m, n;
	cout << "Введите координаты слона:" << endl;
	cin >> k >> l;
	cout << "Введите координаты фигуры:" << endl;
	cin >> m >> n;
	if ((k >= 1 && k <= 9) && (l >= 1 && l <= 9) && (m >= 1 && m <= 9) && (n >= 1 && n <= 9)) {
		if (abs(k - m) == abs(l - n)) {
			cout << "Слон угрожает фигуре." << endl;
		}
		else {
			cout << "Слон не угрожает фигуре." << endl;
		}	
	}
	else {
		cout << "Вы ввели неверные координаты фигур. Введите значения от 1 до 9" << endl;
	}
}