#include <iostream>
#include <locale.h>
#include <cstdio>
#include <cmath>

int main() {
	setlocale(LC_CTYPE, "Russian");
	using namespace std;
	float x, z, j, y = 2.7, a = -5.5e-4;
	for (int i = 0; i < 4; i++) {
		cout << "Введите j: ";
		cin >> j;
		x = pow(cos(y), 2) / (j + 2 * a * y);
		if (x >= sqrt(y)) {
			z = exp(-j);
		}
		else {
			z = pow((0.5 * y / j), 2);
		}
		printf("Значение x=%f\n", x);
		printf("Значение z=%f\n\n", z);
	}
}