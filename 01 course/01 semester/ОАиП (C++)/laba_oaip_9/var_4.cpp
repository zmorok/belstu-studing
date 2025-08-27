#include <iostream>
#include <cmath>

float f(float x) {
	return exp(x) - (1 / x);
}

int main() {
	using namespace std;
	float a = 2, b = 3, n = 200, h, x, s, osn_left, osn_right;
	h = (b - a) / n;
	x = a; 
	s = 0;
	cout << "\n\n\n";
	for (int i = 0; i < n; i++) {
		osn_left = f(x);
		x += h;
		osn_right = f(x);
		s += (h / 2) * (osn_left + osn_right);
	}
	cout << "\t\t\t" << s << "\n\n\n";
}