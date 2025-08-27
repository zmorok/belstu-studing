#include <iostream>
using namespace std;

int main() {
    setlocale(0, "");
    int n;
    cout << "Введите размер массива: ";
    cin >> n;

    float* mas = new float[n];

    cout << "Введите элементы массива:\n";
    for (int i = 0; i < n; i++) {
        cout << "Элемент " << i + 1 << ": ";
        cin >> mas[i];
    }

    int minIndex = 0;
    for (int i = 1; i < n; i++) {
        if (mas[i] < mas[minIndex]) {
            minIndex = i;
        }
    }

    int firstNegativeIndex = -1;
    int secondNegativeIndex = -1;
    for (int i = 0; i < n; i++) {
        if (mas[i] < 0) {
            if (firstNegativeIndex == -1) {
                firstNegativeIndex = i;
            }
            else {
                secondNegativeIndex = i;
                break;
            }
        }
    }

    if (firstNegativeIndex == -1 || secondNegativeIndex == -1) {
        cout << "В массиве не хватает двух отрицательных элементов.\n";
        return 1;
    }

    float sum = 0;
    for (int i = firstNegativeIndex + 1; i < secondNegativeIndex; i++) {
        sum += mas[i];
    }

    cout << "Номер минимального элемента: " << minIndex + 1 << endl;
    cout << "Сумма элементов между первым и вторым отрицательными: " << sum << endl;

    delete[] mas; 
}
