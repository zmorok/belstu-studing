#include <iostream>
#include <cmath>
using namespace std;

float f(float x) {
	return exp(x) - 3 - (1 / x);
}

int main() {
	float a = -1000, b = 1000, e = 0.0001, x = 0;
	cout << endl << endl;
	
	while (abs(b - a) > 2 * e) {
		x = (a + b) / 2;
		if (f(x) * f(a) <= 0) {
			b = x;
		}
		else { a = x; }
	}
	cout << "\t\t\t" << x << "\n\n\n";

}