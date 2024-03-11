#include <iostream>
#include <locale.h>
#include <time.h>
#include <new>

int main() {
	setlocale(LC_ALL, "Russian");
	using namespace std;
	int n, i, k, size;

	cout << "������� ������ �������: ";
	cin >> size;
	n = size;
	cout << "������ � �������� � " << size << " ���������:" << endl;
	int* mas = new int[size]; // ������� ��������� �� ����� ����� � �������� ������ ��� ������� ����� �����
	srand((unsigned)time(NULL));
	for (i = 0; i < n; i++) {
		mas[i] = rand() % 100;
		cout << mas[i] << " ";
	}
	cout << endl << endl;

	cout << "������� ����� �������� (�� 1 �� " << size << "), ������� ������ ������� : ";
	cin >> k;
	cout << "����� ������ ��� " << k <<"-�� ��������: " << endl;
	if (k > size && k < 0) {
		return 1;
	}
	else {
		for (i = 0; i < size-1; i++) {
			if (i < k-1) {
				cout << mas[i] << " ";
			}
			else {
				mas[i] = mas[i + 1];
				cout << mas[i] << " ";
			}
		}
	}
	cout << endl << endl;

	cout << "������ � ������ ����� ������ ���������: " << endl;
	for (i = 0; i < size - 1; i++) {
		if (mas[i] % 2 == 0) {
			size++; // ����������� ������ �� 1 ��� ���������� ����
			int* newMas = new int[size];
			for (int j = 0; j <= i; j++) {
				newMas[j] = mas[j];
			}
			
			newMas[i + 1] = 0;

			for (int j = i + 2; j < size; j++) {
				newMas[j] = mas[j-1];
			}
			delete[] mas; // ����������� ������
			mas = newMas; // �������� ������ � ����� ����� ������� �������� 
			i++; // ���������� ������ ��� ����������� ����
		}	
	}
	for (i = 0; i < size-1; i++) {
		cout << mas[i] << " ";
	}
	cout << endl;
}
