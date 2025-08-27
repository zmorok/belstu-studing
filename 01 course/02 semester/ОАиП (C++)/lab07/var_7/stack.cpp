#include "stack.h"
#include <iostream>
#include <fstream>
using namespace std;

// Добавление элемента в стек
void push(Stack*& top, int data) {
    Stack* newNode = new Stack; // памать под data
    newNode->data = data;       // запись элемента data в data
    newNode->next = top;        // вершина переходит на текущий элемент
    top = newNode;              // сдвиг вершины на позицию вперед
}

// Удаление элемента из стека
int pop(Stack*& top) {
    if (top == nullptr) {
        cerr << "Стек пуст" << endl;
        return -1;
    }
    Stack* temp = top;          // временная переменная для хранения адреса элемента
    int data = top->data;      // запись элемента из поля data в переменную data 
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

// Функция для удаления первого элемента, выше 100
int deleteFirstElementMoreThan100(Stack*& top) {
    if (top == nullptr) {
        return -1;
    }

    int data;
    if (top->data > 100) {
        data = top->data;
        pop(top);
        return data;
    }

    Stack* current = top->next;
    Stack* prev = top;

    while (current != nullptr) {
        if (current->data > 100) {
            data = current->data;
            prev->next = current->next; // Пропускаем элемент current
            delete current;             // Освобождаем память, выделенную под элемент
            return data;
        }
        prev = current;
        current = current->next;
    }
    return -1;
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
    int value;
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
