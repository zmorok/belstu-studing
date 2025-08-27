#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    ifstream inputFile("FILE1.txt");
    ofstream outputFile("FILE2.txt");

    char line[100]; 

    int lineCount = 1;

    while (inputFile.getline(line, 100)) {
        if (lineCount >= 4) {
            outputFile << line << endl;
        }
        lineCount++;
    }

    inputFile.close();
    outputFile.close();

   
    ifstream file2("FILE2.txt");

    
    char lastWord[100]; 

    int lastWordLength = 0;
 
    while (file2 >> lastWord) {
        lastWordLength = 0;
        int i = 0;
        while (lastWord[i] != '\0') {
            lastWordLength++;
            i++;
        }
    }

    file2.close();

    cout << "Количество символов в последнем слове FILE2: " << lastWordLength << endl;
}