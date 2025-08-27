#include <iostream>
#include <cmath>

float f(float x) {
	return exp(x) - (1 / x);
}

int main() {
	using namespace std;
	float a = 2, b = 3, n = 200, h, x, s1, s2, S;
	h = (b - a) / (n * 2);
	x = a + (2 * h);
	s1 = 0;
	s2 = 0;	
	cout << "\n\n\n";
	for (int i = 1; i < n * 2; i++) {
		s2 += f(x);
		x += h;
		s1 += f(x);
		x += h;
		i++;
	}
	S = (h / 3) * (f(a) + 4 * f(a + h) + (4 * s1) + (2 * s2) + f(b));
	cout << "\t\t\t" << S << "\n\n\n";
}