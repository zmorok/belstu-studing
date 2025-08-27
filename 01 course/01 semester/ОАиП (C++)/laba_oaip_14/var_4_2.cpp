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
			cout << "[" << i + 1 << "][" << j + 1 << "]= ";
			cin >> matrix[i][j];
		}
	cout << endl;

	cout << "Ваша матрица: " << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << "[ " << matrix[i][j] << " ]";
		}
		cout << '\n';
	}

	int counter = 0;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (matrix[i][j] != matrix[i][0] && matrix[i][j] != matrix[i][n] && matrix[i][j] > matrix[i][j-1] && matrix[i][j] < matrix[i][j+1])
				counter++;
	cout << "\nКол-во элементов, удовлетворяющих условию= " << counter << endl;

}