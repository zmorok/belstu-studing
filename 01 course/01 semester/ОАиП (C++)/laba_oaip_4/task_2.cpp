#include <iomanip>
#include <iostream>
int main() {
	setlocale(LC_CTYPE, "Russian");
	using namespace std;

	int x = 0;
	int y_1 = x - (x / 2);
	int y_2 = x + (x / 2);
	char s;

	cout << "¬ведите позицию: ";
	cin >> x;
	cout << "¬ведите символ круга: ";
	cin >> s;
	cout << setw(x) << s<< endl;
	for (x > y_1; x--;) {
		cout << setw(x) << setfill(s) << s << endl;
	} 

}