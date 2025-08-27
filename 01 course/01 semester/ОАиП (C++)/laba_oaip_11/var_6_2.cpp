#include <iostream>
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    
    char tmp[32] = "\0";

    int A, n;
    int B, m;
    cout << "Введите число A: "; cin >> A;
    cout << "Введите число B: "; cin >> B;

    _itoa_s(A, tmp, 2);
    cout << "Введите позицию (не больше чем " << strlen(tmp) - 2 << ") в числе A : "; cin >> n;
    
    _itoa_s(B, tmp, 2);
    cout << "Введите позицию (не больше чем " << strlen(tmp) - 2 << ") в числе B : "; cin >> m;

    if (n > strlen(tmp) - 2 && m > strlen(tmp) - 2) {
        return 1;
    }

    cout << endl;

    int maskA = ((1 << 3) - 1) << (n - 1);
    int maskB = ((1 << 3) - 1) << (m - 1);

    _itoa_s(A, tmp, 2);
    cout << "Число A в двоичной системе счислений: " << tmp << endl;

    _itoa_s((A & maskA) >> (n - 1), tmp, 2);
    cout << "Выбранные три бита из числа А: " << tmp << endl;

    _itoa_s(B, tmp, 2);
    cout << "Число B в двоичной системе счислений: " << tmp << endl;

    _itoa_s(B & ~maskB, tmp, 2);
    cout << "Число В с занулёнными битами: " << tmp << endl;

    _itoa_s(((A & maskA) >> (n - 1)) << (m - 1) | B & ~maskB, tmp, 2);
    cout << "Новое число B: " << tmp << endl;
}