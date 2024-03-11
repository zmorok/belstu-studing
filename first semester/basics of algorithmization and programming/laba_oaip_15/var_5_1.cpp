#include <iostream>
using namespace std;

int main() {
    setlocale(0, "");
    int n;

    cout << "������� ������ �������: ";
    cin >> n;

    float* mas = new float[n];

    cout << "������� �������� �������:\n";
    for (int i = 0; i < n; i++) {
        cout << "������� " << i + 1 << ": ";
        cin >> mas[i];
    }

    float minElement = mas[0];
    int minIndex = 0;
    for (int i = 1; i < n; i++) {
        if (mas[i] < minElement) {
            minElement = mas[i];
            minIndex = i;
        }
    }

    int countZeros = 0;
    for (int i = 0; i < n; i++) {
        if (mas[i] == 0) {
            countZeros++;
        }
    }

    float sumAfterMin = 0;
    for (int i = minIndex + 1; i < n; i++) {
        sumAfterMin += mas[i];
    }

    cout << "���������� ���������, ������ 0: " << countZeros << endl;
    cout << "����� ��������� ����� ������������ ��������: " << sumAfterMin << endl;

    delete[] mas;
}
