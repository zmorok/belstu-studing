#include <iostream>
#include <locale.h>

int main() {
	setlocale(LC_CTYPE, "Russian");
	using namespace std;
	int N = 0;
	cout << "Введите номер месяца: ";
	cin >> N;
	switch (N) {
	case 12: puts("Зима"); break;
	case 1: puts("Зима"); break;
	case 2: puts("Зима"); break;
	case 3: puts("Весна"); break;
	case 4: puts("Весна"); break;
	case 5: puts("Весна"); break;
	case 6: puts("Лето"); break;
	case 7: puts("Лето"); break;
	case 8: puts("Лето"); break;
	case 9: puts("Осень"); break;
	case 10: puts("Осень"); break;
	case 11: puts("Осень"); break;
	default: puts("Вы ввели неверный номер месяца"); break;
	}
}