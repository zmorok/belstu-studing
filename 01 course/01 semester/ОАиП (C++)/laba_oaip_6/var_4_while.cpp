#include <iostream>
#include <cmath>
#include <locale.h>

int main() {
	setlocale(LC_CTYPE, "Russian");
	using namespace std;
	float j, b = 2.5, a = 1.4e-3;
	int m = 3;
	while (b <= 3) {
		cout << "Введите j: ";
		cin >> j;
		float y = (m * j) / tan(a) - exp(10 * m);
		cout << "Значение выражения y: " << y << endl;
		float z = 2 * y * b + sqrt(a + b);
		cout << "Значение выражения z: " << z << endl;
		b += 0.1;
	}
}