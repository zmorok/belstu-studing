#include <iostream>
#include <Windows.h>

using namespace std;

int choice = 0;

static void Menu() {
	system("cls");
	cout << "\n\t\tСистема управления данными клиентов:\n\n";
	cout << "\t[1] Добавить нового клиента\n";
	cout << "\t[2] Список клиентов\n";
	cout << "\t[3] Изменить данные клиента\n";
	cout << "\t[4] Составить договор по сделке с клиентом\n";
	cout << "\t[5] Завершить программу\n";
	cout << "\n\tНомер оперцации: ";
	cin >> choice;
}

int main() {
	int client_choice = 0;
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	while (true) {
		Menu();
		switch (choice) {
		case 1:
			system("cls");
			cout << "\n\t\tСистема управления данными клиентов:\n";
			cout << "\n\tДобавление нового клиента    ";
			for (int i = 0; i < 8; ++i) {
				i % 4 == 0 ? cout << "\b \b\b \b\b\b " : cout << '.';
				Sleep(500);
			}
			cout << "\n\n\tКлиент успешно добавлен!\n\t";
			system("pause");
			break;
		case 2:
			system("cls");
			cout << "\n\t\tСистема управления данными клиентов:\n";
			cout << "\n\tСписок клиентов:\n";
			cout << "\n\t1) Клиент №1:" << "\n\tНаименование: " << "\n\tАдрес: " << "\n\tТелефон: " << "\n\tКонтактное лицо :" << endl;
			cout << "\n\t2) Клиент №2:" << "\n\tНаименование: " << "\n\tАдрес: " << "\n\tТелефон: " << "\n\tКонтактное лицо :" << endl;
			system("pause");
			break;
		case 3:
			system("cls");
			cout << "\n\t\tСистема управления данными клиентов:\n";
			cout << "\n\tИзменение данных клиента    ";
			for (int i = 0; i < 8; ++i) {
				i % 4 == 0 ? cout << "\b \b\b \b\b\b " : cout << '.';
				Sleep(500);
			}
			cout << "\n\n\tДанные клиента успешно изменены!\n\t";
			system("pause");
			break;
		case 4:
			client_choice = 0;
			system("cls");
			cout << "\n\t\tСистема управления данными клиентов:\n";
			cout << "\n\tВыберите клиента для составления договора по сделке: ";
			cin >> client_choice;
			cout << "\n\tСоставление нового договора    ";
			for (int i = 0; i < 8; ++i) {
				i % 4 == 0 ? cout << "\b \b\b \b\b\b " : cout << '.';
				Sleep(500);
			}
			cout << "\n\n\tДоговор с клиентом " << client_choice << " успешно составлен! \n\n\t";
			system("pause");
			break;
		case 5:
			system("cls");
			cout << "\n\t\tСистема управления данными клиентов:\n";
			cout << "\n\tПрограмма завершена.\n";
			return 0;
		default:
			cout << "********Неверная операция, повторите программу********\n\t";
			system("pause");
			break;
		}
	}
}