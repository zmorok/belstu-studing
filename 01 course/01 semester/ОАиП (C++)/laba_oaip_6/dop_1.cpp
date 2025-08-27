#include <iostream>
#include <locale.h>

int main() {
	setlocale(LC_CTYPE, "Russian");
	using namespace std;
	float startSum, endSum;
	float finalSum = 0;
	int days = 0;
	cout << "Введите заработок в первый день: "; cin >> startSum;
	cout << "Введите заработок в последний день: "; cin >> endSum; cout << endl;
	while (startSum < endSum) {
		startSum += 0.03 * startSum;
		days += 1;
		finalSum += startSum;
	}
	cout << "Выручка в последний день: " << finalSum << endl;
	cout << "Количество дней, прошедших с первого дня: " << days << endl;
}