#include <iomanip>
#include <iostream>

using namespace std;

int main() {
	setlocale(LC_CTYPE, "Russian");
	int x, y = 0;
	char symb, probel;
	probel = ' ';
	cout << "¬ведите позицию: ";
	cin >> x;
	cout << "¬ведите символ: ";
	cin >> symb;
	while (x > 1) {
		cout << '\t' << '\t' << '\t' << '\t' << '\t' << setw(x - 1) << setfill(probel) << probel;
		cout << setw(y+1) << setfill(symb) << symb << endl;
		x--;
		y += 2;
	}
}