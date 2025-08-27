#include <iostream>
using namespace std;

int main() {
	setlocale(LC_CTYPE, "Russian");
	int i, k, sz = 5;
	float A[] = { 5, -4, 17.1, 9, 1 };
	cout << "Введите номер элемента (от 0 до 4) " << endl;
	cin >> k;
	cout << endl;
	for (i = k; i < sz - 1; i++) {
		*(A + i) = *(A + i + 1);
		sz--;
	}
		
	for (i = 0; i < sz; i++) {
		cout << *(A + i) << endl;
	}
}
