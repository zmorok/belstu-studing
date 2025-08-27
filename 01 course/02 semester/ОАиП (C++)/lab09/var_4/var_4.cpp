#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

struct Person {
    string name;
    string city;
};

struct Node {
    Person data;
    Node* prev;
    Node* next;
};

Node* head = nullptr;
Node* tail = nullptr;

// Добавление в конец списка
static void insert(const Person& person) {
    Node* newNode = new Node;
    newNode->data = person;
    newNode->next = nullptr;

    if (!head) {
        head = tail = newNode;
    }
    else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

// Вывод списка на экран
static void outputList() {
    Node* current = head;
    int i = 0;
    cout << "\nСписок:\n";
    while (current != nullptr) {
        cout << ++i << ". " << current->data.name << " | " << current->data.city << endl;
        current = current->next;
    }
}

// Удаление имени
static void delet(const string& name) {
    Node* current = head;
    while (current != nullptr) {
        if (current->data.name == name) {
            if (current == head) {
                current->prev = nullptr;
                head = current->next;
            }
            if (current == tail) {
                tail = current->prev;
            }
            if (current->prev) {
                current->prev->next = current->next;
            }
            if (current->next) {
                current->next->prev = current->prev;
            }
            delete current;
            return;
        }
        current = current->next;
    }
}

// Поиск по имени
static Node* find(const string& name) {
    Node* current = head;
    while (current != nullptr) {
        if (current->data.name == name) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// Запись в файл
static void writeToFile(const string& filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Ошибка открытия файла " << filename << " для записи." << endl;
        return;
    }
    Node* current = head;
    while (current != nullptr) {
        outFile << current->data.name << " " << current->data.city << endl;
        current = current->next;
    }
    outFile.close();
}

// Чтение с файла
static void readFromFile(const string& filename) {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }

    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Ошибка открытия файла " << filename << "для чтения." << endl;
        return;
    }
    Person person;
    while (inFile >> person.name >> person.city) {
        insert(person);
    }
    inFile.close();
}

// Удаление K последних элементов
static void deleteKLast(int k) {
    if (!tail) {
        cout << "Список пуст. Нет элементов для удаления." << endl;
        return;
    }

    int length = 0;
    Node* currentCount = tail;
    while (currentCount != nullptr) {
        length++;
        if (currentCount == head) break;
        currentCount = currentCount->prev;
    }

    if (length == k) {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        return;
    }
    else if (length < k || k <= 0) {
        cout << "Неверное количество последних элементов для удаления.\n";
        return;
    }

    // Удаление K последних элементов
    Node* current = tail;
    while (current != nullptr && k > 0) {
        Node* prev = current->prev;
        delete current;
        current = prev;
        k--;
    }

    if (current == nullptr) {
        head = nullptr;
        tail = nullptr;
    }
    else {
        tail = current;
        tail->next = nullptr;
    }
}

// Поиск минимального элемента
static Person* findMin() {
    if (head == nullptr) {
        return nullptr;
    }

    Node* current = head;
    Person* minPerson = &current->data;

    while (current != nullptr) {
        if (current->data.name < minPerson->name) {
            minPerson = &current->data;
        }
        current = current->next;
    }

    return minPerson;
}

// Поиск максимального элемента
static Person* findMax() {
    if (head == nullptr) {
        return nullptr;
    }

    Node* current = head;
    Person* maxPerson = &current->data;

    while (current != nullptr) {
        if (current->data.name > maxPerson->name) {
            maxPerson = &current->data;
        }
        current = current->next;
    }

    return maxPerson;
}

// Добавление в начало списка
static void addXBegin(const Person& person) {
    Node* newNode = new Node;
    newNode->data = person;
    newNode->next = nullptr;

    newNode->next = head;
    newNode->prev = nullptr;

    if (head) {
        head->prev = newNode;
    }

    head = newNode;
    if (!tail) {
        tail = head;
    }
}

// Меню
static void displayMenu() {
    cout << "Меню:\n";
    cout << "1. Добавить в конец списка\n";
    cout << "2. Удаление имени\n";
    cout << "3. Удаление K последних имён\n";
    cout << "4. Вывод на экран\n";
    cout << "5. Поиск по имени\n";
    cout << "6. Чтение с файла\n";
    cout << "7. Запись в файл\n";
    cout << "8. Найти минимальное имя\n";
    cout << "9. Найти максимальное имя\n";
    cout << "10. Добавить в начало списка\n";
    cout << "0. Выход\n";
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int choice;
    do {
        system("cls");
        outputList();
        cout << "\n\n";
        displayMenu();
        cout << "Введите выбор: ";
        cin >> choice;
        switch (choice) {
        case 1: {
            Person person;
            cout << "Введите имя: ";
            cin >> person.name;
            cout << "Введите город: ";
            cin >> person.city;
            insert(person);
            break;
        }
        case 2: {
            string name;
            cout << "Введите имя: ";
            cin >> name;
            delet(name);
            break;
        }
        case 3: {
            int k;
            cout << "Введите кол-во последних элементов для удаления: ";
            cin >> k;
            deleteKLast(k);
            break;
        }
        case 4:
            outputList();
            break;
        case 5: {
            string name;
            cout << "Введите имя для поиска: ";
            cin >> name;
            if (find(name)) {
                cout << "\n\t" << find(name)->data.name << " | " << find(name)->data.city << endl;
            }
            else {
                cout << "Пользователь не найден." << endl;
            }
        }
            break;
        case 6: {
            readFromFile("data.txt");
            break;
        }
        case 7: {
            writeToFile("data.txt");
            break;
        }
        case 8: {
            Person* minPerson = findMin();
            if (minPerson) {
                cout << "Минимальное имя: " << minPerson->name << " | " << minPerson->city << endl;
            }
            else {
                cout << "Список пуст." << endl;
            }
            break;
        }
        case 9: {
            Person* maxPerson = findMax();
            if (maxPerson) {
                cout << "Максимальное имя: " << maxPerson->name << " | " << maxPerson->city << endl;
            }
            else {
                cout << "Список пуст." << endl;
            }
            break;
        }
        case 10: {
            Person person;
            cout << "Введите имя: ";
            cin >> person.name;
            cout << "Введите город: ";
            cin >> person.city;
            addXBegin(person);
            break;
        }
        case 0:
            cout << "Выход из программы.\n";
            break;
        default:
            cout << "Неверный ввод.\n";
            break;
        }
        system("pause");
    } while (choice != 0);

    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }

    return 0;
}