#include <iostream>
#include <cmath>
int main() {
	setlocale(LC_CTYPE, "Russian");
	using namespace std;
	float a = 0, b = 0, c = 0;
	cout << "������� a, b, c" << endl;
	cin >> a; cin >> b; cin >> c;
	float d = (b * b) - 4 * a * c;
	if (d < 0) {
		cout << "������ ���" << endl;
	}
	else {
		float x1 = (-b + sqrt(d)) / (2 * a);
		float x2 = (-b - sqrt(d)) / (2 * a);
		cout << "������ ������: " << x1 << endl;
		cout << "������ ������: " << x2 << endl;
	}
}