#include <iostream>
#include <string>
#include <cctype>
#include <Windows.h>
using namespace std;

const int ALPHABET_SIZE = 26;

// массив для хранения количества вхождений каждой буквы в алфавите
int letterCount[ALPHABET_SIZE] = { 0 };

// подсчет количества вхождений каждой буквы в строке
static void countLetters(string& str, int* letterCount) {
    for (char c : str) {
        if (isalpha(c)) {
            c = tolower(c);
            int index = c - 'a';
            letterCount[index]++;
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    string inputString;
    do {
        system("cls");
        cout << "Для выхода из программы введите \"exit\"" << endl;
        cout << "Введите строку: ";
        getline(cin, inputString);
        if (inputString == "exit")
            break;

        // подсчёт количество вхождений каждой буквы в строке
        countLetters(inputString, letterCount);

        cout << "Буквы и их количества в строке:" << endl;
        cout << "Буква\t|\tКоличество" << endl;
        cout << "------------------------" << endl;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            char letter = 'a' + i;
            if (letterCount[i] != 0)
                cout << letter << "\t| +\t" << letterCount[i] << endl;
            else cout << letter << "\t|\t" << letterCount[i] << endl;
        }
        cout << "------------------------" << endl;

        // поиск введенной буквы в алфавите
        char searchLetter;
        cout << "Введите букву для поиска: ";
        cin >> searchLetter;
        searchLetter = tolower(searchLetter);
        if (isalpha(searchLetter)) {
            int index = searchLetter - 'a';
            if (letterCount[index] > 0) {
                cout << "Буква '" << searchLetter << "' встречается в строке " << letterCount[index] << " раз(а)." << endl;
            }
            else {
                cout << "Буква '" << searchLetter << "' не встречается в строке." << endl;
            }
        }
        else {
            cout << "Введите корректную букву." << endl;
        }
        system("pause");
        cin.ignore();
    } while (true);
    return 0;
}