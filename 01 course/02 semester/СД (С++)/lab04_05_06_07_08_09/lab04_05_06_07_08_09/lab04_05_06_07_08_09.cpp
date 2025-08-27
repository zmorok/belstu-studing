#include "functions.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <Windows.h>

using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    readClientsFromFile();
    int choice;
    do {
        system("cls");
        cout << "\n\t\tСистема управления клиентами\n\n";
        cout << "\t\t1. Добавить клиента\n";
        cout << "\t\t2. Отобразить всех клиентов\n";
        cout << "\t\t3. Отобразить клиента по индексу\n";
        cout << "\t\t4. Изменение данных клиентов\n";
        cout << "\t\t5. Удалить клиента\n";
        cout << "\t\t6. Сортировать клиентов\n";
        cout << "\t\t7. Поиск информации по строке\n";
        cout << "\t\t8. Бинарный поиск клиента по индексу\n";
        cout << "\t\t9. Записать клиентов в файл\n";
        cout << "\t\t10. Прочитать клиентов из файла\n";
        cout << "\t\t0. Выход\n";
        cout << "\n\t\tВведите номер операции: ";
        cin >> choice;
        switch (choice) {
        case 1:
            addClient();
            break;
        case 2:
            displayClients();
            break;
        case 3:
            displayClientAtIndex();
            break;
        case 4:
            editClient();
            break;
        case 5:
            deleteClient();
            break;
        case 6:
            sortClients();
            break;
        case 7:
            searchClientBySubstring();
            break;
        case 8:
            searchByBinaryWithIndex();
            break;
        case 9:
            writeClientsToFile();
            break;
        case 10:
            readClientsFromFile();
            cout << "\n\tДанные о клиентах успешно прочитаны из файла.\n\n";
            break;
        case 0:
            cout << "\n\t\tВыход из программы.\n\n";
            Sleep(500);
            return 0;
        default:
            cout << "\t\tНеверная операция. Попробуйте другую операцию.\n\n";
        }
        system("pause");
    } while (choice != 0);
}
