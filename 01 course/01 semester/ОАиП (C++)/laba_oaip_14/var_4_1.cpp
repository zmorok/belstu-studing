#include <iostream>
using namespace std;

int main() {
	setlocale(0, "");

	const int rows = 100, columns = 100;
	int matrix[rows][columns];

	int n;
	cout << "Введите кол-во строк матрицы: "; cin >> n;
	if (n <= 0)
		return 1;

	int m;
	cout << "Введите кол-во строк матрицы: "; cin >> m;
	if (m <= 0)
		return 1;

	cout << "Введите элементы матрицы (" << n << "*" << m << "): " << endl;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			matrix[i][j] = rand() % 100;
		}
	cout << endl;

	int sum = 0;

	cout << "Ваша матрица: " << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << "[ " << matrix[i][j] << " ]";
			sum += matrix[i][j];
		}
		cout << '\n';
	}

	int average = sum / (n * m);
	int counter = 0;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (matrix[i][j] > average)
				counter++;
	cout << "\nСреднее арифметическое всех элементов= " << average;
	cout << "\nКол-во элементов, больших среднее арифметического= " << counter << endl;

}