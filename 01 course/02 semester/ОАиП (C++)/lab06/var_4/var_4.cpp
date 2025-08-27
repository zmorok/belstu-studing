#include <iostream>
#include <fstream>

using namespace std;

// Структура узла списка
struct Node {
    char data;
    Node* next;
};

// Функция для добавления элемента в конец списка
static void append(Node*& head, char newData) {
    Node* newNode = new Node;
    newNode->data = newData;
    newNode->next = nullptr;

    if (head == nullptr) {
        head = newNode;
    }
    else {
        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Функция для удаления элемента из списка
static void remove(Node*& head, char targetData) {
    Node* current = head;
    Node* prev = nullptr;

    while (current != nullptr) {
        if (current->data == targetData) {
            if (prev == nullptr) {
                head = current->next;
            }
            else {
                prev->next = current->next;
            }
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Функция для поиска элемента и вывода его позиции в списке
static void findAndDisplayPosition(Node* head, char targetData) {
    int position = 1;
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->data == targetData) {
            cout << "Символ '" << targetData << "' найден на позиции " << position << endl;
            return;
        }
        temp = temp->next;
        position++;
    }
    cout << "Символ '" << targetData << "' не найден в списке." << endl;
}

// Функция для поиска элемента в списке
static Node* search(Node* head, char targetData) {
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->data == targetData) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

// Функция для вывода списка в консольное окно
static void display(Node* head) {
    Node* temp = head;
    while (temp != nullptr) {
        cout << temp->data << " ";
        temp = temp->next;
    }
    cout << endl;
}

// Функция для записи списка в файл
static void writeToFile(Node* head, const string& filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Не удалось открыть файл: " << filename << endl;
        return;
    }
    Node* temp = head;
    while (temp != nullptr) {
        outFile << temp->data << " ";
        temp = temp->next;
    }
    outFile.close();
}

// Функция для считывания списка из файла
static void readFromFile(Node*& head, const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Не удалось открыть файл: " << filename << endl;
        return;
    }

    // Удаление текущего списка
    Node*& tempHead = head;
    Node* delTemp;
    while (tempHead) {
        delTemp = tempHead;
        tempHead = tempHead->next;
        delete delTemp;
    }

    char newData;
    while (inFile >> newData) {
        append(head, newData);
    }
    inFile.close();
}

// Функция для поиска символа и вывода следующего за ним символа
static void findAndDisplayNext(Node* head, char targetData) {
    Node* result = search(head, targetData);
    if (result != nullptr && result->next != nullptr) {
        cout << "Символ: " << result->data << endl;
        cout << "Следующий символ: " << result->next->data << endl;
    }
    else {
        cout << "Символ не найден или отсутствует следующий символ." << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Rus");
    Node* head = nullptr;

    int choice;
    char searchChar;
    string filename;

    while (true) {
        system("cls");
        cout << "Список: ";
        display(head);
        cout << "Меню:" << endl;
        cout << "1. Добавить элемент в список" << endl;
        cout << "2. Удалить элемент из списка" << endl;
        cout << "3. Найти символ" << endl;
        cout << "4. Найти символ и вывести следующий" << endl;
        cout << "5. Вывести список" << endl;
        cout << "6. Записать список в файл" << endl;
        cout << "7. Считать список из файла" << endl;
        cout << "8. Выход" << endl;
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1:
            system("cls");
            cout << "Введите символ для добавления: ";
            cin >> searchChar;
            append(head, searchChar);
            system("pause");
            break;
        case 2:
            system("cls");
            cout << "Введите символ для удаления: ";
            cin >> searchChar;
            remove(head, searchChar);
            system("pause");
            break;
        case 3:
            system("cls");
            cout << "Введите символ для поиска: ";
            cin >> searchChar;
            findAndDisplayPosition(head, searchChar);
            system("pause");
            break;
        case 4:
            system("cls");
            cout << "Введите символ для поиска: ";
            cin >> searchChar;
            findAndDisplayNext(head, searchChar);
            system("pause");
            break;
        case 5:
            system("cls");
            cout << "Список: ";
            display(head);
            system("pause");
            break;
        case 6:
            system("cls");
            cout << "Введите имя файла для записи: ";
            cin >> filename;
            writeToFile(head, filename);
            system("pause");
            break;
        case 7:
            system("cls");
            cout << "Введите имя файла для чтения: ";
            cin >> filename;
            readFromFile(head, filename);
            system("pause");
            break;
        case 8:
            system("cls");
            cout << "Выход из программы." << endl;
            // Очистка памяти перед выходом
            Node* temp;
            while (head != nullptr) {
                temp = head;
                head = head->next;
                delete temp;
            }
            return 0;
        default:
            cout << "Неверный выбор. Попробуйте еще раз." << endl;
            system("pause");
        }
    }

    return 0;
}