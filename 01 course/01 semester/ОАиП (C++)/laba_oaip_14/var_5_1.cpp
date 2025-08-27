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

	int sum = 0;
	int mult = 1;
	int counter = 0;

	cout << "Ваша матрица: " << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (matrix[i][j] % 2 == 0 && matrix[i][j] / 2 % 2 != 0) {
				sum += matrix[i][j];
				mult *= matrix[i][j];
				counter++;
			}
			cout << "[ " << matrix[i][j] << " ]";
		}
		cout << '\n';
	}
	int* mas = new int[counter];
	counter = 0;
	
	for (int i = 0; i < n; i++) 
		for (int j = 0; j < m; j++) 
			if (matrix[i][j] % 2 == 0 && matrix[i][j] / 2 % 2 != 0)
				mas[counter++] = matrix[i][j];
	
	cout << "mas:";
	for (int i = 0; i < counter; i++)
		cout << mas[i] << " ";

	cout << endl;
	cout << "\nСумма элементов, удовлетворяющих условию= " << sum;
	cout << "\nПроизведение элементов, удовлетворяющих условию= " << mult << endl;
}