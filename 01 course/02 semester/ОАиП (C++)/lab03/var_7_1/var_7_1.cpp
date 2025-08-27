#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;

static bool containsMultipleWords(const char* str) {
    bool insideWord = false;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ') {
            if (!insideWord) {
                insideWord = true;
            }
        }
        else {
            if (insideWord) {
                return true;
            }
            insideWord = false;
        }
    }

    return false;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    ifstream inputFile("FILE1.txt");
    ofstream outputFile("FILE2.txt");

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "Ошибка открытия файлов" << endl;
        return 1;
    }

    char line[100];
    bool foundFiveLetterWord = false;

    while (inputFile.getline(line, sizeof(line))) {
        bool singleWord = true;
        bool containsFiveLetterWord = false;
        bool insideWord = false;
        int wordLength = 0;

        for (int i = 0; line[i] != '\0'; i++) {
            char c = line[i];

            if (c == ' ') {
                if (insideWord) {
                    insideWord = false;
                    if (wordLength == 5) {
                        containsFiveLetterWord = true;
                    }
                    wordLength = 0; // Сбрасываем счетчик символов для нового слова
                }
            }
            else {
                if (!insideWord) {
                    insideWord = true;
                }
                wordLength++;
                if (wordLength > 5) {
                    containsFiveLetterWord = false;
                }
            }

            if (c != ' ') {
                if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))) {
                    singleWord = false;
                    break;
                }
            }
        }

        if (insideWord && wordLength == 5) {
            containsFiveLetterWord = true;
        }

        if (singleWord && !containsMultipleWords(line)) {
            if (containsFiveLetterWord && !foundFiveLetterWord) {
                foundFiveLetterWord = true;
                cout << "Найдено слово длиной в 5 символов в файле FILE2: " << line << endl;
            }
            outputFile << line << endl;
        }
    }

    if (!foundFiveLetterWord) {
        cout << "В файле FILE2 не найдено слово длиной в 5 символов" << endl;
    }

    inputFile.close();
    outputFile.close();
}
