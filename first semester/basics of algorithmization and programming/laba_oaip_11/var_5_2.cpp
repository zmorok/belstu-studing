﻿#include <iostream>
using namespace std;

// установка в 1 n битов в числе А вправо от позиции p, замена ими m битов числа В, начиная с позиции q
int main() {
    setlocale(LC_ALL, "Russian");
    int A, n, p;
    int B, m, q;
    cout << "Введите число A: "; cin >> A;
    cout << "Введите число B: "; cin >> B;

    cout << "Введите кол-во битов числа А для установки в 1 (n): "; cin >> n;
    cout << "Введите позицию для n битов, которые будут браться вправо от заданной позиции: "; cin >> p;

    cout << "Введите кол-во битов числа В для замены битами из числа А (m): "; cin >> m;
    cout << "Введите позицию для m битов, которвые будут браться влево от заданной позиции: "; cin >> q;

    if (p < n && m < n) {
        return 1;
    }

    cout << endl;

    char tmp[33];

    int maskA = ((1 << n) - 1) << (p - n);
    _itoa_s(A, tmp, 2);
    cout << "Число А в двоичной системе счислений: " << tmp << endl;

    _itoa_s(maskA, tmp, 2);
    cout << "Маска для числа А: " << tmp << endl;

    _itoa_s((A & maskA) >> (p - n), tmp, 2);
    cout << n << " битов справа от позиции " << p << " (включено) : " << tmp << endl << endl;


    int maskB = ((1 << m) - 1) << (q - 1);
    _itoa_s(B, tmp, 2);
    cout << "Число В в двоичной системе счислений: " << tmp << endl;

    _itoa_s(maskB, tmp, 2);
    cout << "Маска для числа В для зануления m битов: " << tmp << endl;

    _itoa_s(B & ~maskB, tmp, 2);
    cout << "Число В с занулёнными " << m << " битами влево от позиции " << q << " (включено) : " << tmp << endl << endl;

    _itoa_s((((A & maskA) >> (p - n)) << (q - 1)) | (B & ~maskB), tmp, 2);
    cout << "Новое число В с изменёнными битами: " << tmp << endl << endl;
}

