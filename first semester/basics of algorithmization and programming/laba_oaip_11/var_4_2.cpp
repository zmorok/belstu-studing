#include <iostream>
using namespace std;

// ��������� n ����� � 1 �� ������� p �����
int main() {
    setlocale(LC_ALL, "Russian");
    int A;
    cout << "������� �����: "; cin >> A;

    int n;
    cout << "������� ���-�� �����, ������� ������ ���������� � 1: "; cin >> n;

    int p;
    cout << "������� �������, �� ������� �� ������ ���������� " << n << " ����� �����: "; cin >> p;
    cout << endl;

    char tmp[33];

    _itoa_s(A, tmp, 2);
    cout << "����� " << A << " � �������� �����: " << tmp << endl;

    int mask = (1 << n) - 1;
    _itoa_s(mask, tmp, 2);
    cout << "����� �� " << n << " �����: " << tmp << endl;
    
    mask <<= p;
    _itoa_s(mask, tmp, 2);
    cout << "����� �� ������� " << p << " �����: " << tmp << endl;

    _itoa_s(A | mask, tmp, 2);
    cout << "����� " << A << " c " << n << " ������ ����� �� ������� " << p << " : " << tmp << endl;
}