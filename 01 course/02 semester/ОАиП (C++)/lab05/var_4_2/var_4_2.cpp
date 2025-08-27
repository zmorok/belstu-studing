#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

const int MAX_NAME_LENGTH = 50;

union AccountType {
    char accountType[20];
};

struct Client {
    char fullName[MAX_NAME_LENGTH];
    AccountType type;
    int accountNumber;
    double balance;
};

// ввода данных о клиенте
static Client inputClient() {
    Client client;
    cout << "Введите Ф.И.О. клиента: ";
    cin.ignore();
    cin.getline(client.fullName, MAX_NAME_LENGTH);
    cout << "Введите тип счета: ";
    cin.getline(client.type.accountType, 20);
    cout << "Введите номер счета: ";
    cin >> client.accountNumber;
    cout << "Введите сумму на счете: ";
    cin >> client.balance;
    cin.ignore();
    return client;
}

// запись данных о клиенте в файл в указанном формате
static void writeClientToFile(const Client& client, const char* filename) {
    ofstream outFile(filename, ios::app);
    if (outFile.is_open()) {
        outFile << client.fullName << "," << client.type.accountType << "," << client.accountNumber << "," << client.balance << endl;
        outFile.close();
        cout << "Данные успешно записаны в файл." << endl;
    }
    else {
        cout << "Не удалось открыть файл." << endl;
    }
}

// чтение данных о клиенте из файла
static void readClientFromFile(const char* filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        Client client;
        while (inFile.getline(client.fullName, MAX_NAME_LENGTH, ',') &&
            inFile.getline(client.type.accountType, 20, ',') &&
            (inFile >> client.accountNumber) && (inFile.ignore(), inFile >> client.balance)) {
            cout << "Ф.И.О. клиента: " << client.fullName << endl;
            cout << "Тип счета: " << client.type.accountType << endl;
            cout << "Номер счета: " << client.accountNumber << endl;
            cout << "Сумма на счете: " << client.balance << endl;
            cout << "=========================" << endl << endl;
        }
        inFile.close();
    }
    else {
        cout << "Не удалось открыть файл." << endl;
    }
}

// поиск клиента по номеру счета
static void findClientByAccountNumber(const char* filename, int accountNumber) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        Client client;
        bool found = false;
        while (inFile.getline(client.fullName, MAX_NAME_LENGTH, ',') &&
            inFile.getline(client.type.accountType, 20, ',') &&
            (inFile >> client.accountNumber) && (inFile.ignore(), inFile >> client.balance)) {
            if (client.accountNumber == accountNumber) {
                found = true;
                cout << "Ф.И.О. клиента: " << client.fullName << endl;
                cout << "Тип счета: " << client.type.accountType << endl;
                cout << "Номер счета: " << client.accountNumber << endl;
                cout << "Сумма на счете: " << client.balance << endl;
                cout << "=========================" << endl;
                break;
            }
        }
        if (!found) {
            cout << "Клиент с номером счета " << accountNumber << " не найден." << endl;
        }
        inFile.close();
    }
    else {
        cout << "Не удалось открыть файл." << endl;
    }
}

static void menu() {
    cout << "\nМеню:" << endl;
    cout << "1. Добавить нового клиента" << endl;
    cout << "2. Вывести всех клиентов из файла" << endl;
    cout << "3. Найти клиента по номеру счета" << endl;
    cout << "0. Выйти из программы" << endl;
    cout << "Выберите действие: ";
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    const char* filename = "clients.txt";

    int choice;
    do {
        system("cls");
        menu();
        cin >> choice;

        switch (choice) {
        case 1: {
            system("cls");
            Client newClient = inputClient();
            writeClientToFile(newClient, filename);
            system("pause");
            break;
        }
        case 2: {
            system("cls");
            cout << "Данные о клиентах из файла:\n\n";
            readClientFromFile(filename);
            system("pause");
            break;
        }
        case 3: {
            system("cls");
            int searchAccountNumber;
            cout << "\nВведите номер счета для поиска: ";
            cin >> searchAccountNumber;
            findClientByAccountNumber(filename, searchAccountNumber);
            system("pause");
            break;
        }
        case 0:
            cout << "Программа завершена." << endl;
            break;
        default:
            cout << "Некорректный выбор. Попробуйте снова." << endl;
            system("pause");
            break;
        }
    } while (choice != 0);
}