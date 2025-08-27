#include <iostream>
#include <locale.h>
int main() {
	setlocale(LC_CTYPE, "Russian");
	using namespace std;
	int a, b, r, s;
	cout << "Введите размеры участка (a x b)" << endl;
	cin >> a >> b;
	cout << "Введите размеры дома (r x s)" << endl;
	cin >> r >> s;
	int areaRegion = a * b;
	int areaHouse = r * s;
	if (areaRegion - (areaHouse * 2) >= 0) {
		cout << "На участке можно разместить два дома." << endl;
	}
	else {
		cout << "На участке нельзя разместить два дома." << endl;
	}
}