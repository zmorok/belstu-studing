#include <iostream>
using namespace std;

int main() {
	setlocale(LC_CTYPE, "rus");
	float y = 1.2, p, q, x = 0.4e6;
	int t = 6;
	p = (2.6 * t) + cos(y / ((3 * x) + y));
	q = sin(t) / cos(t);
	cout << "Вариант 4:" << endl;
	cout << "\tp = " << p << endl;
	cout << "\tq = " << q << endl << endl;

	float  x_1 = 1.1, w, v, a = 5e-6;
	int b = 40;
	w = (a + b) * tan(x_1) / (x_1 + 1);
 	v = 1 / 2 * b - sqrt(w - (a * b));
	cout << "Вариант 5:" << endl;
	cout << "\tw= " << w << endl;
	cout << "\tv= " << v << endl << endl;

	float y_1 = 0.956, t_1, u, s;
	int n = 4;
	t_1 = 1 / sqrt(y_1) + (14 * a);
	u = (t_1 + 1) / (a + 2);
	s = log(((2 * n) / 3) + exp(-n) / u);
 	cout << "Вариант 6:" << endl;
 	cout << "\tt= " << t_1 << endl;
	cout << "\tu= " << u << endl;
	cout << "\ts= " << s << endl << endl;

	float x_2 = 1.4, w_1, y_2, z = 0.05e-5;
	int m = 6;
	y_2 = sqrt(1 + x_2) - cos(2 / m);
	w_1 = (0.6 * z) - (2 * exp(-2 * y_2 * m));
	cout << "Вариант 7:" << endl;
	cout << "\ty= " << y_2 << endl;
	cout << "\tw= " << w_1 << endl;
}