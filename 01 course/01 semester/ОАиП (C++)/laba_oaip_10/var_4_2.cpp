#include <iostream>
using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");

	float size;
	int	i;

	cout << "������� ������ �������: ";
	cin >> size;
	cout << endl;
	if (size < 0) {
		cout << "������";
		return 1;
	} 
	else if ((int)size - size != 0) {
		cout << "������";
		return 1;
	}
	
	char* mas = new char[size];
	for (i = 0; i < size; i++) {
		cout << "������� " << i + 1 << "-�� ������� �������: ";
		cin >> mas[i];
	}
	
	cout << endl;

	cout << "\n������ ��������: " << endl;
	for (i = 0; i < size; i++) {
		cout << "'" << *(mas + i) << "'" << " ";
	}

	cout << endl;

	int numCount = 0, charCount = 0, size_new_num = 0, size_new_char = 0;

	int* numMas = new int[size_new_num];
	char* charMas = new char[size_new_char];

	for (i = 0; i < size; i++) {
		if (isdigit(mas[i])) {
			numCount++; 
			size_new_num++;
			numMas[numCount - 1] = mas[i] - '0';
		}
		else {
			charCount++;
			size_new_char++;
			charMas[charCount - 1] = mas[i];
		}
	}

	delete[] mas;

	cout << endl << "������ �����:" << endl;
	if (numCount == 0) {
		cout << "����� ���";
	}
	else {
		for (i = 0; i < size_new_num; i++) {
			cout << numMas[i] << " ";
		}
	}
	
	

	cout << endl << "������ ��������:" << endl;
	if (charCount == 0) {
		cout << "�������� ���";
	}
	else {
		for (i = 0; i < size_new_char; i++) {
			cout << "'" << charMas[i] << "'" << " ";
		}
	}
	
	cout << endl;
	return 0;
}