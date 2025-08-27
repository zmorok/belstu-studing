#include <iostream>
using namespace std;

int main() {
	setlocale(LC_CTYPE, "Russian");
	float n;
	int digit, sum = 0;
	cout << "Введите кол-во чисел: ";
	cin >> n;
	if (n <= 0) {
		cout << "Кол-во чисел должно быть положительным и не равняться нулю!" << endl;
	}
	else if (n - int(n) > 0) {
		cout << "Кол-во чисел должно быть целым, а не дробным" << endl;
	}
	else {
		for (int i = 1; i < n + 1; i++) {
			cout << "Введите число: ";
			cin >> digit;
			if (i % 2 == 0) {
				sum += digit;
			}
			else { continue; }
		}
		cout << "Сумма чётных элементов= " << sum << endl;
	}
}