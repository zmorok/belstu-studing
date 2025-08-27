#include <iostream>
#include <cmath>
#include <locale.h>

int main() {
	setlocale(LC_CTYPE, "Russian");
	using namespace std;
	float j, b = 2.5, a = 1.4e-3;
	int m = 3;
	for (int n = 0; n < 3; n++) {
		cout << "Введите j: ";
		cin >> j;
		float y = (m * j) / tan(a) - exp(10 * m);
		cout << "Значение выражения y: " << y << endl;
		float z = 2 * y * b + sqrt(a + b);
		cout << "Значение выражения z: " << z << endl;
	}
	/*for (int n = 0; n < 3; n++) {
		cout << "Введите j: ";
		cin >> j;
		int s = m + j;
		cout << s << endl;
	}*/
}