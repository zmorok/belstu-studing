#include <iostream>
#include <Windows.h>
using namespace std;

// элемент очереди
struct Node {
    char data;
    Node* next;
};

// абстракция очереди
struct Queue {
    Node* front;
    Node* back;
    int maxSize;
    int currentSize;
};

// функция инициализации пустой очереди
static Queue* createQueue(int size) {
    Queue* queue = new Queue;
    queue->maxSize = size;
    queue->currentSize = 0;
    queue->front = nullptr;
    queue->back = nullptr;
    return queue;
}

// функция проверки пустоты очереди
static bool isEmpty(Queue* queue) {
    return (queue->currentSize == 0);
}

// функция проверки заполненности очереди
static bool isFull(Queue* queue) {
    return (queue->currentSize == queue->maxSize);
}

// функция удаления элемента из начала очереди
static char dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        cout << "Очередь пуста. Невозможно удалить символ." << endl;
        return '\0';
    }

    char value = queue->front->data;
    Node* temp = queue->front;
    queue->front = queue->front->next;
    delete temp;

    queue->currentSize--;

    return value;
}

// функция добавления элемента в конец очереди
static void enqueue(Queue* queue, char value) {
    if (isFull(queue)) {
        cout << "Очередь заполнена. Невозможно добавить символ." << endl;
        return;
    }

    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = nullptr;

    if (!isEmpty(queue) && queue->back->data == value) {
        cout << "Совпадение найдено. Удаленный символ: " << dequeue(queue) << endl;
    }

    if (isEmpty(queue)) {
        queue->front = queue->back = newNode;
    }
    else {
        queue->back->next = newNode;
        queue->back = newNode;
    }

    queue->currentSize++;
}

// функция текущего размера функции
static int size(Queue* queue) {
    return queue->currentSize;
}

// функция вывода очереди
static void display(Queue* queue) {
    if (isEmpty(queue)) {
        cout << "Очередь пуста." << endl;
        return;
    }

    Node* temp = queue->front;
    cout << "Очередь: [";
    while (temp != nullptr) {
        cout << temp->data << ", ";
        temp = temp->next;
    }
    cout << "\b\b]" << endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int maxSize;
    cout << "Введите максимальный размер очереди: ";
    cin >> maxSize;

    Queue* queue = createQueue(maxSize);

    int choice;
    char value;

    do {
        system("cls");
        display(queue);
        cout << "\nОперации очереди:" << endl;
        cout << "1. Добавить в очередь" << endl;
        cout << "2. Удалить с очереди" << endl;
        cout << "3. Отобразить" << endl;
        cout << "4. Размер" << endl;
        cout << "0. Выход" << endl;
        cout << "Введите номер операции: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Введите символ для добавления в очередь: ";
            cin >> value;
            enqueue(queue, value);
            break;
        case 2:
            cout << "Удаленный символ: " << dequeue(queue) << endl;
            break;
        case 3:
            display(queue);
            break;
        case 4:
            cout << "Размер очереди: " << size(queue) << endl;
            break;
        case 0:
            cout << "Выход из программы." << endl;
            break;
        default:
            cout << "Неверная операция. Попробуйте еще раз." << endl;
        }
        system("pause");
    } while (choice != 0);

    delete queue;

    return 0;
}
