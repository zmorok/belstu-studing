#include <iostream>
#include <fstream>

using namespace std;

// генерация чисел длинной A из цифр, не превышающих или равных A
static void genNum(int* current, int index, int length, int A, ofstream& outFile) {
    if (index == length) {
        for (int i = 0; i < length; ++i) {
            outFile << current[i];
        }
        outFile << endl;
        return;
    }

    for (int i = 0; i <= A; i++) {
        current[index] = i;
        genNum(current, index + 1, length, A, outFile);
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    int A;
    cout << "Введите цифру A: ";
    cin >> A;
    int length = A;

    if (A < 0 || A > 9) {
        cerr << "Это не цифра." << '\n';
        return 1;
    }

    ofstream outFile("numbers.txt");
    if (!outFile.is_open()) {
        cout << "Ошибка открытия файла." << '\n';
        return 1;
    }

    int* current = new int[A];
    genNum(current, 0, length, A, outFile);

    cout << "Числа записаны в файл." << endl;
    outFile.close();

    delete[] current;
    return 0;
}