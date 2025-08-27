#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;

void writeToInputFile(const char* inputString);
void readFromInputFile();

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    cout << "Введите строку, состоящую из букв, цифр, запятых, точек, знаков + и -: ";
    char inputString[100];
    cin.getline(inputString, sizeof(inputString));

    writeToInputFile(inputString);
    readFromInputFile();
}

// функция для записи в файл
void writeToInputFile(const char* inputString) {
    ofstream outputFile("input.txt");
    if (outputFile.is_open()) {
        outputFile << inputString;
        outputFile.close();
    }
    else {
        cerr << "Невозможно открыть файл для записи" << endl;
    }
}

// функция для чтения из файла
void readFromInputFile() {
    ifstream inputFile("input.txt");
    if (inputFile.is_open()) {
        char fileContent[100];
        inputFile.getline(fileContent, sizeof(fileContent));

        char integerSubstring[100];
        int integerSubstringIndex = 0;
        bool foundInteger = false;

        for (int i = 0; fileContent[i] != '\0'; i++) {
            char c = fileContent[i];
            if ((c >= '0' && c <= '9') || (c == '+' && integerSubstringIndex == 0) || (c == '-' && integerSubstringIndex == 0)) {
                integerSubstring[integerSubstringIndex++] = c;
            }
            else if (integerSubstringIndex > 0) {
                foundInteger = true;
                break;
            }
        }

        inputFile.close();

        if (foundInteger) {
            integerSubstring[integerSubstringIndex++] = '\0';
            cout << "Подстрока, соответствующая целому числу: " << integerSubstring << endl;
        }
        else {
            cout << "В файле не найдено целого числа" << endl;
        }
    }
    else {
        cerr << "Невозможно открыть файл для чтения" << endl;
    }
}
