#include <iostream>
using namespace std;

static int** transform(int** matrix, int rows, int cols) {
	for (int j = 0; j < cols; ++j) {
		matrix[0][j] -= matrix[cols - 1][j];
	}

	for (int i = 1; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			matrix[i][j] -= matrix[i - 1][j];
		}
	}
	return matrix;
}

static int** fillingCells(int** matrix, int rows, int cols) {
	srand(time(NULL));
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			matrix[i][j] = rand() % 101;
		}
	}
	return matrix;
}

int main() {
	setlocale(0, "");

	int rows, cols;

	cout << "Введите кол-во строк: "; cin >> rows;
	cout << "Введите кол-во столбцов: "; cin >> cols;

	int** matrix = new int* [rows];
	for (int i = 0; i < rows; i++) {
		matrix[i] = new int[cols];
	}

	fillingCells(matrix, rows, cols);

	cout << "Матрица до преобразования: " << endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << "[ " << matrix[i][j] << " ]";
		}
		cout << endl;
	}

	transform(matrix, rows, cols);

	cout << "Матрица после преобразования: " << endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << "[ " << matrix[i][j] << " ]";
		}
		cout << endl;
	}

	delete[] matrix;
}