#include <iostream>
#include <fstream>

using namespace std;

// Структура узла списка
struct Node {
    int data;
    Node* next;
};

// Функция для добавления элемента в конец списка
static void append(Node*& head, int newData) {
    Node* newNode = new Node;
    newNode->data = newData;
    newNode->next = nullptr;

    if (head == nullptr) { // если список пустой, то в 'голову' устанавливается узел newNode
        head = newNode;
    }
    else { // если список не пустой, (head идёт по всему списку) то последнему ptr указывается ссылка на newNode
        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Функция для удаления элемента из списка
static void remove(Node*& head, int targetData) {
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

// Функция для вывода списка в консольное окно
static void display(Node* head) {
    Node* temp = head;
    while (temp != nullptr) {
        cout << temp->data << " ";
        temp = temp->next;
    }
    cout << endl;
}

// Функция для поиска элемента и вывода его позиции в списке
static void findAndDisplayPosition(Node* head, int targetData) {
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

    int newData;
    while (inFile >> newData) {
        append(head, newData);
    }
    inFile.close();
}

// Функция для поиска суммы положительных двузначных элементов в списке
static int sumPositiveTwoDigitElements(Node* head) {
    int sum = 0;
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->data > 9 && temp->data < 100) {
            sum += temp->data;
        }
        temp = temp->next;
    }
    return sum;
}

// Функция для поиска суммы отрицательных двузначных элементов в списке
static int sumNegativeTwoDigitElements(Node* head) {
    int sum = 0;
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->data > -100 && temp->data < -9) {
            sum += temp->data;
        }
        temp = temp->next;
    }
    return sum;
}

// Функция для поиска суммы отрицательных элементов с последней цифрой 3
static int sumNegativeElementsEndingWith3(Node* head) {
    int sum = 0;
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->data < 0 && abs(temp->data) % 10 == 3) {
            sum += temp->data;
        }
        temp = temp->next;
    }
    return sum;
}

int main() {
    setlocale(LC_ALL, "Rus");
    Node* head = nullptr;
    Node* result = nullptr;

    int choice;
    int newData;
    int searchValue;
    string filename;

    while (true) {
        system("cls");
        cout << "Список: ";
        display(head);
        cout << "Меню:" << endl;
        cout << "1. Добавить элемент в список" << endl;
        cout << "2. Удалить элемент из списка" << endl;
        cout << "3. Вывести список" << endl;
        cout << "4. Найти символ" << endl;
        cout << "5. Записать список в файл" << endl;
        cout << "6. Считать список из файла" << endl;
        cout << "7. Найти сумму положительных двузначных элементов" << endl;
        cout << "8. Найти сумму отрицательных двузначных элементов" << endl;
        cout << "9. Найти сумму отрицательных элементов,у которых последняя цифра 3" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1:
            system("cls");
            cout << "Введите целое число для добавления: ";
            cin >> newData;
            append(head, newData);
            system("pause");
            break;
        case 2:
            system("cls");
            cout << "Введите целое число для удаления: ";
            cin >> searchValue;
            remove(head, searchValue);
            system("pause");
            break;
        case 3:
            system("cls");
            cout << "Список: ";
            display(head);
            system("pause");
            break;
        case 4:
            system("cls");
            cout << "Введите элемент для поиска: ";
            cin >> searchValue;
            findAndDisplayPosition(head, searchValue);
            system("pause");
            break;
        case 5:
            system("cls");
            cout << "Введите имя файла для записи: ";
            cin >> filename;
            writeToFile(head, filename);
            system("pause");
            break;
        case 6:
            system("cls");
            cout << "Введите имя файла для чтения: ";
            cin >> filename;
            readFromFile(head, filename);
            system("pause");
            break;
        case 7:
            system("cls");
            {
                int sum = sumPositiveTwoDigitElements(head);
                if (sum > 0) { cout << "Сумма положительных двузначных элементов: " << sum << endl; }
                else { cout << "В списке нет положительных двузначных элементов." << endl; }
            }
            system("pause");
            break;
        case 8:
            system("cls");
            {
                int sum = sumNegativeTwoDigitElements(head);
                if (sum < 0) { cout << "Сумма отрицательных двузначных элементов: " << sum << endl; }
                else { cout << "В списке нет отрицательных двузначных элементов." << endl; }
            }
            system("pause");
            break;
        case 9:
            system("cls");
            {
                int sum = sumNegativeElementsEndingWith3(head);
                if (sum != 0) {
                    cout << "Сумма отрицательных элементов, у которых последняя цифра 3: " << sum << endl;
                }
                else {
                    cout << "В списке нет отрицательных элементов, у которых последняя цифра 3." << endl;
                }
            }
            system("pause");
            break;
        case 0:
            system("cls");
            cout << "Выход из программы." << endl;
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