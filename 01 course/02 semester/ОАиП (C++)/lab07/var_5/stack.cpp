#include "stack.h"
#include <fstream>
#include <iostream>
#include <Windows.h>

using namespace std;

// Создание пустого стека
Stack* createStack() {
    Stack* stack = new Stack;
    stack->top = nullptr;
    return stack;
}

// Добавление элемента в стек
void push(Stack* stack, int data) {
    Node* newNode = new Node;
    newNode->data = data;
    newNode->next = stack->top;
    stack->top = newNode;
}

// Удаление элемента из стека
int pop(Stack* stack) {
    if (stack->top == nullptr) {
        cerr << "Stack is empty" << endl;
        return -1;
    }
    Node* temp = stack->top;
    int popped = temp->data;
    stack->top = temp->next;
    delete temp;
    return popped;
}

// Очистка стека
void clear(Stack* stack) {
    while (stack->top != nullptr) {
        pop(stack);
    }
}

// Запись стека в файл
void saveToFile(Stack* stack, const char* filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла." << endl;
        return;
    }
    Node* current = stack->top;
    while (current != nullptr) {
        file << current->data << " ";
        current = current->next;
    }
    file.close();
}

// Чтение стека с файла
Stack* loadFromFile(Stack* stack, const char* filename) {
    clear(stack);
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла." << endl;
        return nullptr;
    }

    Stack* temp = createStack();
    int value;
    while (file >> value) {
        push(temp, value);
    }
    file.close();

    while (temp->top != nullptr) {
        push(stack, pop(temp));
    }

    clear(temp);
    return stack;
}

// Вывод стека
void printStack(Stack* stack) {
    Node* current = stack->top;
    while (current != nullptr) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;
}

void printStack(Stack* stack, Stack* stack1, Stack* stack2) {
    clear(stack);
    Node* current = stack1->top;
    while (current != nullptr) {
        if (!isExistInStack(stack2, current->data)) {
            push(stack, current->data);
        }
        current = current->next;
    }

    Stack* reversed = createStack();
    while (stack->top != nullptr) {
        push(reversed, pop(stack));
    }

    Node* current_ = reversed->top;
    while (current_ != nullptr) {
        cout << current_->data << " ";
        current_ = current_->next;
    }
    cout << endl;

    clear(reversed);
}
//===============================================

bool isExistInStack(Stack* stack, int data) {
    Node* current = stack->top;
    while (current != nullptr) {
        if (current->data == data) {
            return true;
        }
        current = current->next;
    }
    return false;
}