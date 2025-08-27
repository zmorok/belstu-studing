#include <iostream>
#include <fstream>
#include <string>
#include <locale.h>

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

    Queue() {
        front = nullptr;
        back = nullptr;
    }
};

// функция инициализации пустой очереди
static Queue* createQueue() {
    return new Queue();
}

// функция добавления элемента в конец очереди
static void enqueue(Queue* queue, char value) {
    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = nullptr;

    if (queue->back == nullptr) {
        queue->front = queue->back = newNode;
    }
    else {
        queue->back->next = newNode;
        queue->back = newNode;
    }
}

// функция удаления элемента из начала очереди
static char dequeue(Queue* queue) {
    if (queue->front == nullptr) {
        cerr << "Очередь пуста. Невозможно удалить элемент." << endl;
        return '\0';
    }

    char value = queue->front->data;
    Node* temp = queue->front;
    queue->front = queue->front->next;
    if (queue->front == nullptr) {
        queue->back = nullptr;
    }
    delete temp;

    return value;
}

// функция перемещения цифр в конец строки
static string moveDigitsToEnd(const string& line) {
    Queue* digitsQueue = createQueue();
    string result; 

    for (char c : line) {
        if (isdigit(c)) {
            enqueue(digitsQueue, c);
        }
        else {
            result += c;
        }
    }

    while (digitsQueue->front != nullptr) {
        result += dequeue(digitsQueue);
    }

    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");
    ifstream inputFile("f.txt");
    ofstream outputFile("g.txt");

    if (!inputFile || !outputFile) {
        cerr << "Не удалось открыть файлы." << endl;
        return 1;
    }

    string line;
    while (getline(inputFile, line)) {
        string modifiedLine = moveDigitsToEnd(line);
        outputFile << modifiedLine << endl;
    }

    cout << "Преобразование завершено." << endl;

    inputFile.close();
    outputFile.close();

    return 0;
}
