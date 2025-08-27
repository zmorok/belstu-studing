#include <iostream>
#include <Windows.h>
using namespace std;

enum Days {
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
	SUNDAY
};

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int choice = 0;
	while (true) {
		system("cls");
		cout << "Введите номер дня ('-1' - exit): ";
		cin >> choice;

		Days day;
		switch (choice) {
		case -1:
			cout << "\n\tОстановка программы...\n";
			return 0;
		case 1:
			day = MONDAY;
			break;
		case 2:
			day = TUESDAY;
			break;
		case 3:
			day = WEDNESDAY;
			break;
		case 4:
			day = THURSDAY;
			break;
		case 5:
			day = FRIDAY;
			break;
		case 6:
			day = SATURDAY;
			break;
		case 7:
			day = SUNDAY;
			break;
		default:
			cout << "\n\tНеверное число. Повторите попытку...\n\n";
			Sleep(1000);
			continue;
		}

		cout << "\nДень недели: ";
		switch (day) {
		case MONDAY:
			cout << "Понедельник\n\n";
			break;
		case TUESDAY:
			cout << "Вторник\n\n";
			break;
		case WEDNESDAY:
			cout << "Среда\n\n";
			break;
		case THURSDAY:
			cout << "Четверг\n\n";
			break;
		case FRIDAY:
			cout << "Пятница\n\n";
			break;
		case SATURDAY:
			cout << "Субботв\n\n";
			break;
		case SUNDAY:
			cout << "Воскресенье\n\n";
			break;
		}
		system("pause");
	}
}