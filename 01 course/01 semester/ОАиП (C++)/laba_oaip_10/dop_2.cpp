#include <iostream>
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    int n;
    cout << "Введите размер массива: ";
    cin >> n;

    if (n <= 1) {
        cout << "Массив слишком маленький для поиска пар" << endl;
        return 1;
    }

    int* mas = new int[n];
    int counter = 0;

    cout << "Введите элементы массива:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> mas[i];
    }

    for (int i = 1; i < n; i++) {
        if (mas[i] == mas[i - 1]) {
            counter++;
        }
    }

    cout << "Количество пар соседних элементов с одинаковыми значениями: " << counter << endl;

    return 0;
}
