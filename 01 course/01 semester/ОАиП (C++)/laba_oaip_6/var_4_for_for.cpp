#include <iostream>
#include <cmath>
#include <locale.h>

int main() {
	setlocale(LC_CTYPE, "Russian");
	using namespace std;
	float j, m, b = 2.5, a = 1.4e-3;
	for (int n1 = 0; n1 < 3; n1++) {
		cout << "Введите m: ";
		cin >> m;
		for (int n2 = 0; n2 < 4; n2++) {
			cout << "Введите j: ";
			cin >> j;
			float y = (m * j) / tan(a) - exp(10 * m);
			cout << "Значение выражения y: " << y << endl;
			float z = 2 * y * b + sqrt(a + b);
			cout << "Значение выражения z: " << z << endl;
			if (n2 == 4) {
				cout << endl;
			}
		}
	}
}