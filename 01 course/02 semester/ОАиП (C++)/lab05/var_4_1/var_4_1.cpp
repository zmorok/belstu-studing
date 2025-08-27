#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

const int MAX_CLIENTS = 100;

// тип счета
enum AccountType {
    SAVINGS,
    CURRENT,
    FIXED_DEPOSIT,
    OTHER
};

// информация о клиенте
struct BankClient {
    string fullName;
    AccountType accountType;
    int accountNumber;
    double balance;
    struct {
        unsigned int day : 5;
        unsigned int month : 4;
        unsigned int year : 12;
    } lastUpdateDate;
};

BankClient clients[MAX_CLIENTS];
int numClients = 0;

// ввод данных о клиенте
static void inputClient() {
    if (numClients < MAX_CLIENTS) {
        BankClient& client = clients[numClients];
        cout << "Введите ФИО: ";
        getline(cin, client.fullName);
        cout << "Введите тип счёта (0 - Расчётный, 1 - Текущий, 2 - Срочный, 3 - Другой): ";
        int type = 0;
        cin >> type;
        if (type != 0 && 1 && 2 && 3) {
            cout << "Error\n";
            return;
        }
        client.accountType = static_cast<AccountType>(type);
        cout << "Введите номер счёта: ";
        cin >> client.accountNumber;
        cout << "Введите баланс: ";
        cin >> client.balance;
        cin.ignore();
        cout << "Введите дату последнего обновления счёта (ДД ММ ГГГГ): ";
        int day, month, year;
        cin >> day >> month >> year;

        client.lastUpdateDate.day = day;
        client.lastUpdateDate.month = month;
        client.lastUpdateDate.year = year;
        cin.ignore();
        numClients++;
    }
    else {
        cout << "Максимальное кол-во клиентов достигнуто." << endl;
    }
}

// вывод информации о клиенте
static void displayClient(BankClient& client) {
    cout << "ФИО: " << client.fullName << endl;
    cout << "Тип счёта: ";
    switch (client.accountType) {
    case SAVINGS:
        cout << "Расчётный";
        break;
    case CURRENT:
        cout << "Текущий";
        break;
    case FIXED_DEPOSIT:
        cout << "Срочный";
        break;
    case OTHER:
        cout << "Другой";
        break;
    }
    cout << endl;
    cout << "Номер счёта: " << client.accountNumber << endl;
    cout << "Баланс: " << client.balance << endl;
    cout << "Дата последнего обновления: " << client.lastUpdateDate.day << "/" << client.lastUpdateDate.month << "/" << client.lastUpdateDate.year << endl;
}

// удаление клиента
static void deleteClient(int accountNumber) {
    cout << endl << endl;
    for (int i = 0; i < numClients; ++i) {
        if (clients[i].accountNumber == accountNumber) {
            for (int j = i; j < numClients - 1; ++j) {
                clients[j] = clients[j + 1];
            }
            numClients--;
            cout << "Клиент с номером счета " << accountNumber << " был удалён." << endl;
            return;
        }
    }
    cout << "Клиент с номером счета " << accountNumber << " не найден." << endl;
}

// поиск клиента по диапазону суммы
static void searchByBalance(double minBalance, double maxBalance) {
    bool found = false;
    cout << "Клиенты с балансом между " << minBalance << " и " << maxBalance << ":" << endl;
    for (int i = 0; i < numClients; ++i) {
        if (clients[i].balance >= minBalance && clients[i].balance <= maxBalance) {
            displayClient(clients[i]);
            cout << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Клиентов с таким балансом не найдено." << endl;
    }
}

// меню
static void menu() {
    system("cls");
    cout << "\nМеню:\n1. Добавить клиента\n2. Отобразить клиентов\n3. Удалить клиента\n4. Поиск по балансу\n0. Выход\nВведите номер операции: ";
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    clients[numClients++] = { "Клиент 1", SAVINGS, 1001, 5000.0, {15, 10, 2023} };
    clients[numClients++] = { "Клиент 2", CURRENT, 1002, 3000.0, {20, 12, 2022} };


    int choice;
    do {
        menu();
        cin >> choice;
        cin.ignore();
        switch (choice) {
        case 1: {
            system("cls");
            inputClient();
            system("pause");
            break;
        }
        case 2: {
            system("cls");
            cout << "\nСписок клиентов:\n";
            for (int i = 0; i < numClients; ++i) {
                displayClient(clients[i]);
                cout << endl;
            }
            system("pause");
            break;
        }
        case 3: {
            system("cls");
            cout << "\nСписок клиентов:\n";
            for (int i = 0; i < numClients; ++i) {
                displayClient(clients[i]);
                cout << endl;
            }
            int accountNumber;
            cout << "Введите номер счёта для удаления: ";
            cin >> accountNumber;
            deleteClient(accountNumber);
            system("pause");
            break;
        }
        case 4: {
            system("cls");
            float minBalance, maxBalance;
            cout << "Введите минимальный баланс: ";
            cin >> minBalance;
            cout << "Введите максимальный баланс: ";
            cin >> maxBalance;
            searchByBalance(minBalance, maxBalance);
            system("pause");
            break;
        }
        case 0:
            cout << "Выход...";
            break;
        default:
            system("cls");
            cout << "Неверная операция. Повторите снова." << endl;
            system("pause");
        }
    } while (choice != 0);
}