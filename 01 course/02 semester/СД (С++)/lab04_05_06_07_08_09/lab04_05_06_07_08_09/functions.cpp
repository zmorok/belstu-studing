#include "functions.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <Windows.h>

using namespace std;

// Структура клиента
struct Client {
    int index;
    string name;
    string address;
    string phoneNumber;
    string company;
};


const int MAX_CLIENTS = 100;
Client clients[MAX_CLIENTS];
int numClients = 0;

// Функция добавления клиента (redacted)
void addClient() {
    system("cls");
    cout << "\n\t\tДобавление клиента...\n\n";
    if (numClients < MAX_CLIENTS) {
        Client newClient;

        newClient.index = numClients;

        cout << "\tВведите имя клиента: ";
        cin.ignore();
        getline(cin, newClient.name);

        cout << "\tВведите адрес клиента: ";
        getline(cin, newClient.address);

        cout << "\tВведите телефон клиента: ";
        getline(cin, newClient.phoneNumber);

        cout << "\tВведите наименование компании клиента: ";
        getline(cin, newClient.company);

        clients[numClients++] = newClient;
        cout << "\n\tКлиент добавлен успешно!\n\n";
    }
    else {
        cout << "Хранилище клиентов переполнено. Невозможно добавить нового клиента.\n";
    }
}

// Функция отображения клиентов
 void displayClients() {
    system("cls");
    cout << "\n\tСписок клиентов:\n\n";
    cout << "Индекс\tИмя\t\t\tАдрес\t\t\tТелефон\t\t\tКомпания\n";
    for (int i = 0; i < numClients; i++) {
        string tabs = clients[i].name.length() > 15 ? "\t" : "\t\t";
        cout << clients[i].index << "\t" <<
            clients[i].name << tabs <<
            clients[i].address << "\t\t" <<
            clients[i].phoneNumber << "\t\t" <<
            clients[i].company << "\n";
    }
    cout << '\n';
}

// Функция поиска клиента по индексу (redacted)
 void displayClientAtIndex() {
     system("cls");
     int index;
     cout << "\n\tВведите индекс клиента для отображения: ";
     cin >> index;
     for (const auto& client : clients) {
         if (client.index == index) {
             cout << "Индекс: " << client.index <<
                 "\n\tИмя: " << client.name <<
                 "\n\tАдрес: " << client.address <<
                 "\n\tТелефон: " << client.phoneNumber <<
                 "\n\tКомпания: " << client.company << "\n\n";
             return;
         }
     }

     cout << "Клиент с указанным индексом не найден.\n";
 }

// Функция удаления клиента по индексу 
 void deleteClient() {
     system("cls");
     displayClients();
     int index;
     cout << "Введите индекс клиента для удаления: ";
     cin >> index;

     bool found = false;
     for (int i = 0; i < numClients; ++i) {
         if (clients[i].index == index) {
             found = true;
             numClients--;
             for (; i < numClients; i++) {
                 clients[i] = clients[i + 1];
             }
             cout << "\tКлиент был удален успешно.\n";
             break;
         }
     }

     if (!found) {
         cout << "\tКлиент с указанным индексом не найден.\n";
     }
 }

// Функция сортировки клиентов по выбранному параметру и порядку
 void sortClients() {
    system("cls");
    int sortChoice;
    cout << "\n\tВыберите параметр для сортировки:\n";
    cout << "\t1. По имени\n";
    cout << "\t2. По адресу\n";
    cout << "\t3. По номеру телефона\n";
    cout << "\t4. По компании\n";
    cout << "\n\tВведите номер параметра: ";
    cin >> sortChoice;

    int orderChoice;
    cout << "\n\tВыберите порядок сортировки:\n";
    cout << "\t1. По возрастанию\n";
    cout << "\t2. По убыванию\n";
    cout << "\n\tВведите номер порядка: ";
    cin >> orderChoice;

    bool ascending = (orderChoice == 1);
    // Выбор функции сравнения в зависимости от выбора пользователя
    bool (*compareFunction)(const Client&, const Client&);
    switch (sortChoice) {
    case 1:
        compareFunction = [](const Client& a, const Client& b) {
            return a.name < b.name;
            };
        break;
    case 2:
        compareFunction = [](const Client& a, const Client& b) {
            return a.address < b.address;
            };
        break;
    case 3:
        compareFunction = [](const Client& a, const Client& b) {
            return a.phoneNumber < b.phoneNumber;
            };
        break;
    case 4:
        compareFunction = [](const Client& a, const Client& b) {
            return a.company < b.company;
            };
        break;
    default:
        cout << "\n\tНеверный выбор. Возврат к главному меню.\n\n";
        return;
    }

    // Сортировка клиентов пузырьком
    for (int i = 0; i < numClients - 1; i++) {
        for (int j = 0; j < numClients - i - 1; j++) {
            if (compareFunction(clients[j], clients[j + 1]) == !ascending) {
                swap(clients[j], clients[j + 1]);
            }
        }
    }

    cout << "\n\tСписок клиентов после сортировки:\n\n";
    cout << "Индекс\tИмя\t\tАдрес\t\tТелефон\t\tКомпания\n";
    for (int i = 0; i < numClients; i++) {
        cout << clients[i].index << "\t" <<
            clients[i].name << "\t" <<
            clients[i].address << "\t" <<
            clients[i].phoneNumber << "\t" <<
            clients[i].company << "\n";
    }
    cout << '\n';
}

// Функция записи данных в файла (сортировка по индексу и обновление)
 void writeClientsToFile() {
    system("cls");
    ofstream file("clients.txt");
    if (file.is_open()) {
        for (int i = 0; i < numClients; i++) {
            file << i << "," << clients[i].name <<
                "," << clients[i].address <<
                "," << clients[i].phoneNumber <<
                "," << clients[i].company << endl;
        }
        file.close();
        cout << "\n\tДанные о клиентах сохранены в файл успешно.\n\n";
        system("pause");
    }
    else {
        cout << "\n\tНе удалось открыть файл для записи.\n\n";
        system("pause");
    }
    readClientsFromFile();
}

// Функция чтения данных из файла (redacted)
 void readClientsFromFile() {
    system("cls");
    ifstream file("clients.txt");
    if (file.is_open()) {
        numClients = 0;
        string line;
        while (getline(file, line)) {
            string sym = ",";
            size_t pos = 0;
            Client client;
            // Индекс
            pos = line.find(sym);
            int index = stoi(line.substr(0, pos));
            client.index = index;
            line.erase(0, pos + sym.length());
            // Имя
            pos = line.find(sym);
            client.name = line.substr(0, pos);
            line.erase(0, pos + sym.length());
            // Адрес
            pos = line.find(sym);
            client.address = line.substr(0, pos);
            line.erase(0, pos + sym.length());
            // Телефон
            pos = line.find(sym);
            client.phoneNumber = line.substr(0, pos);
            line.erase(0, pos + sym.length());
            // Компания
            pos = line.find(sym);
            client.company = line.substr(0, pos);
            line.erase(0, pos + sym.length());
            clients[index] = client;
            numClients++;
        }
        file.close();
    }
    else {
        cout << "\n\tНе удалось открыть файл для чтения.\n\n";
    }
}

// Функция изменения данных пользователя
 void editClient() {
    system("cls");
    displayClients();
    int index;
    cout << "Введите индекс клиента для редактирования: ";
    cin >> index;
    if (index >= 0 && index < numClients) {
        Client& client = clients[index];
        cout << "Выберите способ редактирования данных:\n";
        cout << "1. Полное редактирование\n";
        cout << "2. Редактирование отдельных полей\n";
        cout << "Введите номер операции: ";
        int choice;
        cin >> choice;
        switch (choice) {
        case 1:
            cout << "Текущие данные клиента:\n";
            cout << "Имя: " << client.name << "\n";
            cout << "Адрес: " << client.address << "\n";
            cout << "Телефон: " << client.phoneNumber << "\n";
            cout << "Компания: " << client.company << "\n\n";

            cout << "Введите новые данные:\n";
            cout << "Имя: ";
            cin.ignore();
            getline(cin, client.name);
            cout << "Адрес: ";
            getline(cin, client.address);
            cout << "Телефон: ";
            getline(cin, client.phoneNumber);
            cout << "Компания: ";
            getline(cin, client.company);

            cout << "\nДанные клиента успешно изменены.\n";
            break;
        case 2:
        {
            cout << "Выберите поле для редактирования:\n";
            cout << "1. Имя\n";
            cout << "2. Адрес\n";
            cout << "3. Телефон\n";
            cout << "4. Компания\n";
            cout << "Введите номер поля: ";
            int fieldChoice;
            cin >> fieldChoice;
            cout << "Введите новое значение: ";
            string newValue;
            cin.ignore();
            getline(cin, newValue);
            switch (fieldChoice) {
            case 1:
                client.name = newValue;
                break;
            case 2:
                client.address = newValue;
                break;
            case 3:
                client.phoneNumber = newValue;
                break;
            case 4:
                client.company = newValue;
                break;
            default:
                cout << "Некорректный выбор поля.\n";
                break;
            }
            cout << "\nДанные клиента успешно изменены.\n";
            break;
        }
        default:
            cout << "Некорректный выбор операции.\n";
            break;
        }
    }
    else {
        cout << "Некорректный индекс клиента.\n";
    }
}

 // Функция поиска информации по строке
void searchClientBySubstring() {
     system("cls");
     string substring;
     cout << "\nВведите подстроку для поиска: ";
     cin >> substring;

     cout << "\nРезультаты поиска:\n";
     for (const auto& client : clients) {
         if (client.name.find(substring) != string::npos ||
             client.address.find(substring) != string::npos ||
             client.phoneNumber.find(substring) != string::npos ||
             client.company.find(substring) != string::npos) {

             string tabs = client.name.length() > 15 ? "\t" : "\t\t";
             cout << "\t" <<
                 client.index << "\t" << 
                 client.name << tabs <<
                 client.address << "\t\t" <<
                 client.phoneNumber << "\t\t" <<
                 client.company << "\n";;
         }
     }
     cout << '\n';
 }

// Бинарный поиск индекса
static int binarySearchByIndex(Client clients[], int size, int key) {
    
    // Сортировка массива по индексу
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (clients[j].index > clients[j + 1].index) {
                swap(clients[j], clients[j + 1]);
            }
        }
    }

    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (clients[mid].index == key) {
            return mid;
        }

        if (key < clients[mid].index) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }
    return -1; 
}

// Функция бинарного поиска по индексу
void searchByBinaryWithIndex() {
    system("cls");
    int key;
    cout << "\n\tВведите индекс для бинарного поиска: ";
    cin >> key;
    int index = binarySearchByIndex(clients, numClients, key);
    if (index != -1) {
        int clientBinaryIndex = clients[index].index;
        for (const auto& client : clients) {
            if (client.index == clientBinaryIndex) {
                cout << "Индекс: " << client.index <<
                    "\n\tИмя: " << client.name <<
                    "\n\tАдрес: " << client.address <<
                    "\n\tТелефон: " << client.phoneNumber <<
                    "\n\tКомпания: " << client.company << "\n\n";
                return;
            }
        }
    }
    else {
        cout << "Клиент с указанным индексом не найден.\n";
    }
}