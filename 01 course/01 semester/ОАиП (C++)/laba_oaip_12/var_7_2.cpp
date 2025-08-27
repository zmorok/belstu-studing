#include <iostream>
using namespace std;

int main() {
	setlocale(0, "");
	

	int num;
	char tmp[33];
	cout << "Введите число: "; cin >> num;
	_itoa_s(num, tmp, 10);

	int numAf = num;

	int k;
	cout << "Введите цифру, которой должны быть кратны цифры числа " << num << " : "; cin >> k;
	
	int size = 0;

	for (int i = 0; i < strlen(tmp); i++) {
		if ((num % 10) % k == 0) {
			size++;
			num /= 10;
		}
		else {
			num /= 10;
		}
	}

	int* mas = new int(size);

	for (int i = 0; i < size; i++) {
		if ((numAf % 10) % 7 == 0) {
			*(mas + i) = i;
			numAf /= 10;
		}
		else {
			numAf /= 10;
		}
	}

	

	cout << endl;
	for (int i = 0; i < size; i++) {
		cout << *(mas + (i - 1)) << " ";
	}
}