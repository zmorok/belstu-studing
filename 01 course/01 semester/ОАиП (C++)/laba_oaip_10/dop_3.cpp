#include <iostream>
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    int n;
    cout << "Введите размер массива: ";
    cin >> n;

    if (n == 0) {
        cout << "Массив пуст." << endl;
        return 1;
    }

    int* mas = new int[n];

    cout << "Введите элементы массива:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> mas[i];
    }

    int maxStreak = 1;
    int currentStreak = 1; 

    for (int i = 1; i < n; i++) {
        if (mas[i] == mas[i - 1]) {
            currentStreak++;
            if (currentStreak > maxStreak) {
                maxStreak = currentStreak;
            }
        }
        else {
            currentStreak = 1;
        }
    }

    cout << "Наибольшее число подряд идущих одинаковых элементов: " << maxStreak << endl;

    return 0;
}
