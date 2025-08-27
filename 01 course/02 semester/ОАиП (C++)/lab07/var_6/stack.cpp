#include "stack.h"
#include <iostream>
#include <fstream>
using namespace std;

// Добавление элемента в стек
void push(Stack*& top, char data) {
    Stack* newNode = new Stack; // памать под data
    newNode->data = data;       // запись элемента data в data
    newNode->next = top;        // вершина переходит на текущий элемент
    top = newNode;              // сдвиг вершины на позицию вперед
}

// Удаление элемента из вершины стека
int pop(Stack*& top) {
    if (top == nullptr) {
        cerr << "Стек пуст" << endl;
        return -1;
    }
    Stack* temp = top;          // временная переменная для хранения адреса элемента
    char data = top->data;      // запись элемента из поля data в переменную data 
    top = top->next;            // перенос вершины вниз
    delete temp;                // удаление адреса элемента
    return data;                // возвращение значения удаленного элемента
}

// Отчистка стека
void clear(Stack*& top) {
    while (top != nullptr) {
        pop(top);
    }
}

// Функция для подсчета количества элементов стека с равными "соседями"
int countNeighborsWithSameValue(Stack*& top) {
    if (top == nullptr) {
        return 0;
    }

    int count = 0;
    Stack* current = top;
    while (current->next != nullptr) {
        if (current->data == current->next->data) {
            count++;
        }
        current = current->next;
    }
    return count;
}

// Запись стека в файл
void saveToFile(Stack* top, const char* filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла для записи" << endl;
        return;
    }
    Stack* current = top;
    while (current != nullptr) {
        file << current->data << endl;
        current = current->next;
    }
    file.close();
}

// Чтение стека с файла в правильном порядке
Stack* loadFromFile(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла." << endl;
        return nullptr;
    }

    Stack* top = nullptr;
    char value;
    while (file >> value) {
        push(top, value);
    }
    file.close();

    Stack* reversedStack = nullptr;
    while (top != nullptr) {
        push(reversedStack, pop(top));
    }

    return reversedStack;
}

// Вывод стека
void printStack(Stack* top) {
    if (top == nullptr) {
        cout << "Стек пуст" << endl;
        return;
    }
    Stack* current = top;
    while (current != nullptr) {
        cout << "   " << current->data << endl;
        current = current->next;
    }
}
