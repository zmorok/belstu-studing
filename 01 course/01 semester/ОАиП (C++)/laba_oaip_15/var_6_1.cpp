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
    for (int i = 1; i < n; i++) {
        if (mas[i] < minElement) {
            minElement = mas[i];
        }
    }

    int firstPositiveIndex = -1;
    int lastPositiveIndex = -1;
    for (int i = 0; i < n; i++) {
        if (mas[i] > 0) {
            if (firstPositiveIndex == -1) {
                firstPositiveIndex = i;
            }
            lastPositiveIndex = i;
        }
    }

    if (firstPositiveIndex == -1) {
        cout << "� ������� ��� ������������� ���������.\n";
        delete[] mas;
        return 1;
    }

    double sumBetweenPositives = 0;
    for (int i = firstPositiveIndex + 1; i < lastPositiveIndex; ++i) {
        sumBetweenPositives += mas[i];
    }

    cout << "����������� ������� �������: " << minElement << endl;
    cout << "����� ��������� ����� ������ � ��������� ��������������: " << sumBetweenPositives << endl;

    delete[] mas;
}
