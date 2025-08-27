#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;

static void writeToFile(const char* filename, const char* inputBuffer);
static void readFromFile(const char* filename);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    char inputBuffer[1000];
    cout << "������� ������, ��������� �� ���� � ����, ����������� ���������: ";
    cin.getline(inputBuffer, sizeof(inputBuffer));

    writeToFile("input.txt", inputBuffer);
    readFromFile("input.txt");
}

// ������� ��� ������ ������ � ����
static void writeToFile(const char* filename, const char* inputBuffer) {
    ofstream file(filename);
    if (file.is_open()) {
        file << inputBuffer;
        file.close();
    }
    else {
        cerr << "�� ������� ������� ���� ��� ������." << endl;
    }
}

// ������� ��� ������ �� �����
static void readFromFile(const char* filename) {
    ifstream inputFile(filename); // ������
    if (inputFile.is_open()) {
        char wordBuffer[100];
        char longestWord[100];
        int maxWordLength = 0;
        while (inputFile >> wordBuffer) {
            int currentWordLength = 0;
            for (int i = 0; wordBuffer[i] != '\0'; i++)
                ++currentWordLength;
            if (currentWordLength > maxWordLength) {
                maxWordLength = currentWordLength;
                for (int i = 0; i < currentWordLength; i++) {
                    longestWord[i] = wordBuffer[i];
                }
                longestWord[currentWordLength] = '\0';
            }
        }
        inputFile.close();
        cout << "����� ������� �����: " << longestWord << endl;
        cout << "���������� �������� � ����� ������� �����: " << maxWordLength << endl;
    }
    else {
        cerr << "�� ������� ������� ���� ��� ������." << endl;
    }
}