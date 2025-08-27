#include <iostream> 
using namespace std;
int main()
{
	setlocale(LC_CTYPE, "Russian");
	int A = 186; // 10111010
	int B = 167; // 10100111
	char tmp[33];
	int maskA = 14;
	int maskB = ~maskA >> 1;
	cout << "Первое число А=" << A << endl;
	cout << "Второе число В=" << B << endl;

	_itoa_s(A, tmp, 2);
	cout << "A=" << tmp << endl;
	// вывод числа А в двоичной форме

	_itoa_s(B, tmp, 2);
	cout << "B=" << tmp << endl;
	// вывод числа В в двоичной форме

	_itoa_s(maskA, tmp, 2);
	cout << "Маска для А: " << tmp << endl;
	// вывод маски для числа А в двоичной форме, для нахлждения битов

	_itoa_s((A & maskA) >> 1, tmp, 2);
	cout << "Выделенные биты А: " << tmp << endl;
	// побитовое И для числа А и его маски, после сдвиг вправо на 1 бит для удаления нуля

	_itoa_s(maskB, tmp, 2);
	cout << "Маска для В: " << tmp << endl;
	//

	_itoa_s(B & maskB, tmp, 2);
	cout << " Очищены биты в B: " << tmp << endl;

	_itoa_s(((B & maskB) | ((A & maskA)  >> 1)), tmp, 2);
	cout << " Результат B=" << tmp << endl;



	/*
		3 бита числа A начиная со второго по счёту справа = ****101*;
		число B с тремя битами числа А начиная с первого по счёту справа = 10000011;
	*/
}

