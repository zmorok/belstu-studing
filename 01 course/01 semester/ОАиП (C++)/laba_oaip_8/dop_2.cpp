#include <iostream>
using namespace std;

int main() {
	setlocale(LC_CTYPE, "Russian");
	float n;
	int digit, index;
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
			if (digit < 0) {
				index = i;
			}
		}
		cout << "Индекс последнего отрицательного числа= " << index << endl;
	}
}