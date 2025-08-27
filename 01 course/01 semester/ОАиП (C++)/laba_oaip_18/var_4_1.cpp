#include <iostream>
#include <cmath>
using namespace std;

static float function_1(float x) {
	return x * x * x + 2 * x - 1;
}

static float function_2(float x) {
	return exp(x) - 2;
}

static float solving(float (*func)(float), float a, float b, float e = 0.0001, float x = 0) {
	while (abs(b - a) > 2 * e) {
		x = (a + b) / 2;
		if (func(x) * func(a) <= 0) {
			b = x;
		}
		else { a = x; }
	}

	return x;
}

int main() {
	setlocale(0, "");
	float a = -1000, b = 1000;
	cout << endl << endl;

	float x_1 = solving(function_1, a, b);
	cout << "\t\t\tКорень для первого уравнения: " << x_1 << "\n\n\n";
	float x_2 = solving(function_2, a, b);
	cout << "\t\t\tКорень для второго уравнения: " << x_2 << "\n\n\n";	 
}