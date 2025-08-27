#include <iostream>
using namespace std;

// число кратно 4
int main() {
	setlocale(LC_ALL, "Russian");
	int A;
	cout << "Введите ваше число: "; cin >> A;

	if ((A & 7) == 0) {
		cout << "Число " << A << " кратно четырем." << endl;
	}
	else {
		cout << "Число " << A << " НЕ кратно четырем." << endl;
	}
}