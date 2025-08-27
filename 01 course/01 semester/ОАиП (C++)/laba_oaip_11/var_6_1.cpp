#include <iostream>
using namespace std;

// установка в 1 каждый второй бит числа
int main() {
    setlocale(LC_ALL, "Russian");
    int A;
    cout << "Введите число А: "; cin >> A;

    cout << endl;

    char tmp[33];

    _itoa_s(A, tmp, 2);
    cout << "Число А в двоичной системе счислений: " << tmp << endl;
    
    for (int i = 1; i < strlen(tmp); i += 2) {
        int mask = 1 << i;
        A |= mask;
    }

    _itoa_s(A, tmp, 2);
    cout << "Новое число А в двоичной системе: " << tmp << endl;
}

