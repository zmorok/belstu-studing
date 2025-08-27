#include <iostream>
#include <locale.h>
using namespace std;

// разбиение прямоугольника a * b на большие квадраты и вывод кол-во квадратов
static int numsq(int a, int b) {
    if (a == b && a != b != 0) {
        return 1;
    }
    else if (a == 0 || b == 0) {
        return 0;
    }
    else if (a > b) {
        int squares = a / b;
        int remainder = a % b;
        if (b > remainder) {
            return squares;
        }
        else {
            return squares + numsq(b, remainder);
        }
    }
    else {
        int squares = b / a;
        int remainder = b % a;
        if (a > remainder) {
            return squares;
        }
        else {
            return squares + numsq(a, remainder);
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    int a = 0, b = 0;

    while (true) {
        system("cls");
        cout << "Для выхода введите -1" << "\n\n";

        cout << "Введите длину a: ";
        cin >> a;
        if (a == -1) {
            cout << "Выход из программы." << "\n\n";
            system("pause");
            break;
        }
        
        cout << "Введите длину b: ";
        cin >> b;
        if (b == -1) {
            cout << "Выход из программы." << "\n\n";
            break; 
            system("pause");
        }

        if (a < 0 || b < 0) {
            cerr << "Неверные значения." << "\n\n";
            system("pause");
            continue;
        }

        int result = numsq(a, b);
        cout << "\nКоличество квадратов: " << result << "\n\n";
        system("pause");
    }

    return 0;
}