#include <iostream>
using namespace std;

// установка n битов в 1 от позиции p влево
int main() {
    setlocale(LC_ALL, "Russian");
    int A;
    cout << "Введите число: "; cin >> A;

    int n;
    cout << "Введите кол-во битов, которые хотите установить в 1: "; cin >> n;

    int p;
    cout << "Введите позицию, от которой вы хотите установить " << n << " битов ВЛЕВО: "; cin >> p;
    cout << endl;

    char tmp[33];

    _itoa_s(A, tmp, 2);
    cout << "Число " << A << " в двоичной форме: " << tmp << endl;

    int mask = (1 << n) - 1;
    _itoa_s(mask, tmp, 2);
    cout << "Маска из " << n << " битов: " << tmp << endl;
    
    mask <<= p;
    _itoa_s(mask, tmp, 2);
    cout << "Маска от позиции " << p << " слева: " << tmp << endl;

    _itoa_s(A | mask, tmp, 2);
    cout << "Число " << A << " c " << n << " битами слева от позиции " << p << " : " << tmp << endl;
}