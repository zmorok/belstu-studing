#include <iostream>
using namespace std;

// извлечение 4 битов числа A, начиная с пятого по счету справа, и добавление их к числу B справа
int main() {
	setlocale(0, "");
	int A, B;
	int maskA = ((1 << 4) - 1) << 4;
	char tmp[33];

	cout << "Введите число А (>=16): "; cin >> A;
	cout << "Введите число B: "; cin >> B;

	_itoa_s(maskA, tmp, 2);
	cout << "Маска для A: " << tmp << endl;

	_itoa_s(A, tmp, 2);
	cout << "Число А в двоичной системе счислений: " << tmp << endl;
	_itoa_s(B, tmp, 2);
	cout << "Число B в двоичной системе счислений: " << tmp << endl;

	_itoa_s((A & maskA) >> 4, tmp, 2);
	cout << "Взятые 4 бита влево от 5ой позиции числа A: " << tmp << endl;

	_itoa_s(((A & maskA) >> 4) | (B << 4), tmp, 2);
	cout << "Число B с 4 взятыми битами в начале: " << tmp << endl;
}