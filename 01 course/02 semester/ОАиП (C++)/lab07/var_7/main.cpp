#include "stack.h"
#include <iostream>
#include <Windows.h>
using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Stack* top = nullptr;
    int choice;
    do {
        system("cls");
        cout << "Текущий стек:\n=======\n";
        printStack(top);
        cout << "=======\n";
        cout <<
            "1. Добавить элемент\n" <<
            "2. Удалить элемент\n" <<
            "3. Удалить первый элемент больше 100\n" <<
            "4. Записать в файл\n" <<
            "5. Прочитать с файла\n" <<
            "6. Отобразить стек\n" <<
            "7. Очистить стек\n" <<
            "0. Выход\n";
        cout << "Введите номер операции: ";
        cin >> choice;
        switch (choice) {
        case 1: {
            system("cls");
            int data;
            cout << "Введите элемент для добавления: ";
            cin >> data;

            push(top, data);
            system("pause");
            break;
        }
        case 2: {
            system("cls");
            int popped = pop(top);
            if (popped != -1) {
                cout << "Удаленный элемент: " << popped << endl;
            }
            system("pause");
            break;
        }
        case 3: {
            system("cls");
            int popped = deleteFirstElementMoreThan100(top);
            if (popped != -1) {
                cout << "Первый удалённый элемент больше 100: " << popped << endl;
            }
            else {
                cout << "В стеке не было элементов больше 100" << endl;
            }
            system("pause");
            break;
        }
        case 4: {
            system("cls");
            saveToFile(top, "stack.txt");
            cout << "Стек сохранен в файл 'stack.txt'." << endl;
            system("pause");
            break;
        }
        case 5: {
            system("cls");
            clear(top);
            top = loadFromFile("stack.txt");
            cout << "Стек загружен из файла 'stack.txt'." << endl;
            system("pause");
            break;
        }
        case 6: {
            system("cls");
            cout << "Элементы стека:" << endl;
            printStack(top);
            system("pause");
            break;
        }
        case 7: {
            system("cls");
            clear(top);
            cout << "Стек очищен." << endl;
            system("pause");
            break;
        }
        case 0:
            cout << "Выход из программы." << endl;
            break;
        default:
            cout << "Неверный ввод, выберите другой вариант." << endl;
            system("pause");
            break;
        }
    } while (choice != 0);

    return 0;
}
