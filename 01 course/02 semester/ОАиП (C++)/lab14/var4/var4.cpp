#include <iostream>
#include <string>
#include <chrono>
#include <Windows.h>
using namespace std;

int tableSize;

// структура книги
struct Node {
    int bookNumber;
    string bookTitle;
    Node* next;
    Node(int number, const string& title) : bookNumber(number), bookTitle(title), next(nullptr) {}
};

// универсальная хеш-функция
static int universalHash(int key) {
    return floor(tableSize * fmod((key * ((sqrt(5) - 1) / 2)), 1.0));
}

// вставка книги
static void insert(Node** table, int key, const string& title) {
    int index = universalHash(key);
    Node* newNode = new Node(key, title);
    if (table[index] == nullptr) {
        table[index] = newNode;
    }
    else {
        newNode->next = table[index];
        table[index] = newNode;
    }
}

// удаление книги
static void remove(Node* table[], int key) {
    int index = universalHash(key);
    Node* current = table[index];
    Node* prev = nullptr;
    while (current != nullptr) {
        if (current->bookNumber == key) {
            if (prev == nullptr) {
                table[index] = current->next;
            }
            else {
                prev->next = current->next;
            }
            delete current;
            cout << "Книга с номером " << key << " удалена из библиотеки." << endl;
            return;
        }
        prev = current;
        current = current->next;
    }
    cout << "Книга с номером " << key << " не найдена в библиотеке." << endl;
}

// поиск книги по номеру
static string search(Node* table[], int key) {
    int index = universalHash(key);
    Node* current = table[index];
    string result = "";
    while (current != nullptr) {
        if (current->bookNumber == key) {
            if (!result.empty()) {
                result += ", ";
            }
            result += current->bookTitle;
        }
        current = current->next;
    }
    if (result.empty()) {
        result = "Книга не найдена";
    }
    return result;
}

// вывод библиотеки
static void printTable(Node* table[]) {
    cout << "Библиотека:" << endl;
    cout << "Индекс   |  Номер книги\t|    Название книги\t| Заполнено" << endl;
    cout << "--------------------------------------" << endl;
    for (int i = 0; i < tableSize; i++) {
        cout << i << "\t | ";
        if (table[i] != nullptr) {
            Node* current = table[i];
            while (current != nullptr) {
                cout << '\t' << current->bookNumber << "\t\b | " << current->bookTitle << "\t\t\t| Да" << endl;
                current = current->next;
                if (current != nullptr) {
                    cout << "\t | ";
                }
            }
        }
        else {
            cout << "-\t\t\b | -\t\t\t\b | Нет" << endl;
        }
    }
    cout << "--------------------------------------" << endl;
}

// функция для измерения времени выполнения поиска
static double measureSearchTime(Node* table[], int key) {
    auto start = chrono::high_resolution_clock::now();
    search(table, key);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    return duration.count();
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    cout << "Введите размер библиотеки: ";
    cin >> tableSize;

    Node** table = new Node * [tableSize] {nullptr};

    int choice;
    do {
        system("cls");
        printTable(table);
        cout << "Меню:" << endl;
        cout << "1. Добавить книгу" << endl;
        cout << "2. Найти книгу по номеру" << endl;
        cout << "3. Удалить книгу по номеру" << endl;
        cout << "0. Выйти из программы" << endl;
        cout << "Введите номер действия: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int bookNumber;
            string bookTitle;
            cout << "Введите номер книги: ";
            cin >> bookNumber;
            if (bookNumber > -1) {
                cout << "Введите название книги: ";
                cin.ignore();
                getline(cin, bookTitle);
                if (bookTitle != "") {
                    insert(table, bookNumber, bookTitle);
                    cout << "Книга добавлена в библиотеку." << endl;
                } else {
                    cout << "Нужно ввести название книги." << endl;
                }
            }
            else {
                cout << "Неверный номер книги." << endl;
            }
            break;
        }
        case 2: {
            int searchKey;
            cout << "Введите номер книги для поиска: ";
            cin >> searchKey;
            cout << "Результат поиска: " << search(table, searchKey) << endl;
            cout << "Время выполнения поиска: " << measureSearchTime(table, searchKey) << " секунд" << endl;
            break;
        }
        case 3: {
            int removeKey;
            cout << "Введите номер книги для удаления: ";
            cin >> removeKey;
            if (removeKey > -1) {
                remove(table, removeKey);
            }
            else {
                cout << "Неверный номер книги." << endl;
            }
            break;
        }
        case 0: {
            cout << "Выход из программы." << endl;
            break;
        }
        default:
            cout << "Некорректный ввод. Попробуйте снова." << endl;
        }
        system("pause");
    } while (choice != 0);

    for (int i = 0; i < tableSize; i++) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] table;
    return 0;
}
