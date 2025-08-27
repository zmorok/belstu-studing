#include <iostream>
#include <cmath>
int main() {
	setlocale(LC_CTYPE, "Russian");
	using namespace std;
	float a = 0, b = 0, c = 0;
	cout << "Введите a, b, c" << endl;
	cin >> a; cin >> b; cin >> c;
	float d = (b * b) - 4 * a * c;
	if (d < 0) {
		cout << "Корней нет" << endl;
	}
	else {
		float x1 = (-b + sqrt(d)) / (2 * a);
		float x2 = (-b - sqrt(d)) / (2 * a);
		cout << "Первый корень: " << x1 << endl;
		cout << "Первый корень: " << x2 << endl;
	}
}