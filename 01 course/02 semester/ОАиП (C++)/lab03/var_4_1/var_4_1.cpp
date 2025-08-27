#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int counter = 0;

    ifstream file1("FILE1.txt"); // чтение
    ofstream file2("FILE2.txt"); // запись

    char buffer[64];

    if (!file1.is_open() || !file2.is_open()) {
        cerr << "Не удалось открыть файлы." << endl;
        return 1;
    }

    cout << "FILE1 и FILE2 открыты для работы!" << endl << endl;

    while (file1.getline(buffer, 64)) {
        
        bool hasDigits = false;
        for (int d = 0; buffer[d] != '\0'; d++) {
            if (isdigit(buffer[d])) {
                hasDigits = true;
                break;
            }
        }

        if (!hasDigits) {
            file2 << buffer << endl;
        }
    }

    file1.close();
    file2.close();

    ifstream file22("FILE2.txt");

    while (file22.getline(buffer,64)) {
        if (buffer[0] == 'A')
            counter++;
    }

    file2.close();

    cout << "FILE1 и FILE2 закрыты для работы!" << endl << endl;
    cout << "Кол-во строк в FILE2= " << counter << endl;
}
