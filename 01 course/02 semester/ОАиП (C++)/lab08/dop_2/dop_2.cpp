#include <iostream>
#include <locale.h>
using namespace std;

// элемент очереди
struct Node {
    float data;
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

// функция добавления элемента в конец очереди
static void enqueue(Queue* queue, float value) {
    if (isFull(queue)) {
        cout << "Очередь заполнена. Невозможно добавить элемент." << endl;
        return;
    }

    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = nullptr;

    if (isEmpty(queue)) {
        queue->front = queue->back = newNode;
    }
    else {
        queue->back->next = newNode;
        queue->back = newNode;
    }

    queue->currentSize++;
}

// функция удаления элемента из начала очереди
static float dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        cout << "Очередь пуста. Невозможно удалить элемент." << endl;
        return 0.0f;
    }

    float value = queue->front->data; 
    Node* temp = queue->front;
    queue->front = queue->front->next;
    delete temp;

    queue->currentSize--;

    return value;
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

// функция для нахождения индекса максимального элемента в очереди
static int indexOfMax(Queue* queue) {
    if (isEmpty(queue)) {
        return -1;
    }

    int index = 0;
    int maxIndex = 0;
    float maxValue = queue->front->data;
    Node* temp = queue->front->next;

    while (temp != nullptr) {
        index++;
        if (temp->data > maxValue) {
            maxValue = temp->data;
            maxIndex = index;
        }
        temp = temp->next;
    }

    return maxIndex;
}

// функция для выполнения циклического сдвига элементов так, чтобы наибольший элемент был в начале очереди
static void cyclicShiftMaxToFront(Queue* queue) {
    int maxIndex = indexOfMax(queue);
    if (maxIndex == -1 || maxIndex == 0) {
        return;
    }

    // находим элемент, предшествующий максимальному
    Node* beforeMax = queue->front;
    for (int i = 0; i < maxIndex - 1; ++i) {
        beforeMax = beforeMax->next;
    }

    // перемещаем максимальный элемент в начало очереди
    queue->back->next = queue->front;
    queue->front = beforeMax->next;
    beforeMax->next = nullptr;
    queue->back = beforeMax;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int maxSize;
    cout << "Введите максимальный размер очереди: ";
    cin >> maxSize;

    Queue* queue = createQueue(maxSize);

    int choice;
    float value;
    do {
        system("cls");
        display(queue);
        cout << "Меню:" << endl;
        cout << "1. Добавить элемент в очередь" << endl;
        cout << "2. Удалить элемент из очереди" << endl;
        cout << "3. Показать очередь" << endl;
        cout << "4. Выполнить циклический сдвиг" << endl;
        cout << "0. Выйти из программы" << endl;
        cout << "Введите номер операции: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Введите элемент для добавления в очередь: ";
            cin >> value;
            enqueue(queue, value);
            break;
        case 2:
            cout << "Удаленный элемент из очереди: " << dequeue(queue) << endl;
            break;
        case 3:
            cout << "Текущая очередь:" << endl;
            display(queue);
            break;
        case 4:
            cyclicShiftMaxToFront(queue);
            cout << "Выполнен циклический сдвиг, чтобы наибольший элемент был в начале очереди." << endl;
            break;
        case 0:
            cout << "Программа завершена." << endl;
            break;
        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
            break;
        }
        system("pause");
    } while (choice != 0);
    delete queue;

    return 0;
}
