#include <iostream>

int main() {
	using namespace std;
	setlocale(LC_CTYPE, "Russian");

 	cout << "\t\t������� 1" << endl;
	float a[5] = { 2.3, 7, -2, -4, 9 }, k = 6, sum = 0, f, g;
	for (int i = 0; i < 5; i++) {
		sum += (a[i] / (i+1));	
	}
	while (k < 7) {
		f = k;
		g = f / sum;
		cout << "f = k = " << f << endl;
		cout << "sum =  " << sum << endl;
		cout << "g = " << g << endl << endl;
		k += 0.2;
		k = round(k * 10) / 10;
	}

	cout << "\t\t������� 2" << endl;
	float y[5] = { 6, 8, 0.9, 0.2, 4 }, q = 1, temp, new_y;

	cout << "�������� ������: [  ";
	for (int x = 0; x < 5; x++) {
		cout << y[x] << "  ";
	} cout << "]" << endl;

	for (int j = 0; j < 5; j++) {
		temp = y[j]; cout << "����������� ����������� �������: " << temp << endl;
		new_y = y[j] * 2; cout << "����������� ������� * 2: " << new_y << endl;
		int max = new_y;
		for (int w = 0; w < 5; w++) {
			if (y[w] > max) {
				max = y[w];
			}
		} cout << "������������ ������� ������ �������: " << max << endl;
		q *= (temp + max); cout << "������������ " << j+1 << " = " << q << endl << endl;
	}
}