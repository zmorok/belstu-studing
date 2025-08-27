#include <iostream>
#include <locale.h>
using namespace std;

static int A(int m, int n) {
    if (m == 0) {
        return n + 1;
    }
    else if (m > 0 && n > 0) {
        return A(m - 1, n - 1);
    }
    else if (n == 0) {
        return A(m - 1, 1);
    }
    else {
        return -1;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    int m, n;

    while (true) {
        system("cls");
        cout << "Для выхода введите -1" << "\n\n";

        cout << "Введите m: ";
        cin >> m;
        if (m == -1) {
            cout << "Выход из программы." << "\n\n";
            system("pause");
            break;
        }

        cout << "Введите n: ";
        cin >> n;
        if (n == -1) {
            cout << "Выход из программы." << "\n\n";
            system("pause");
            break;
        }

        int result = A(m, n);
        if (result != -1) {
            cout << "Результат рекурсивного алгоритма: " << result << "\n\n";
            system("pause");
        }
        else {
            cout << "Неверные данные. Введите целые неотрицательные значения." << "\n\n";
            system("pause");
        }
    }

    return 0;
}