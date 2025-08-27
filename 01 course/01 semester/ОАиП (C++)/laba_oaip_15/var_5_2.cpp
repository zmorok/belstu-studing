#include <iostream>
using namespace std;

int main() {
    setlocale(0, "");
    int rows, cols;

    cout << "Введите количество строк матрицы: ";
    cin >> rows;

    cout << "Введите количество столбцов матрицы: ";
    cin >> cols;

    int** matrix = new int* [rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }

    cout << "Введите элементы матрицы:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << "Элемент [" << i + 1 << "][" << j + 1 << "]: ";
            cin >> matrix[i][j];
        }
        cout << endl;
    }

    int countColumnsWithZero = 0;
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            if (matrix[i][j] == 0) {
                countColumnsWithZero++;
                break;
            }
        }
    }

    int maxSeriesRow = -1;
    int maxSeriesLength = 0;

    for (int i = 0; i < rows; i++) {
        int currentLength = 1;
        for (int j = 1; j < cols; ++j) {
            if (matrix[i][j] == matrix[i][j - 1]) {
                currentLength++;
            }
            else {
                currentLength = 1;
            }

            if (currentLength > maxSeriesLength) {
                maxSeriesLength = currentLength;
                maxSeriesRow = i;
            }
        }
    }

    
    cout << "Количество столбцов с хотя бы одним нулевым элементом: " << countColumnsWithZero << endl;
    if (maxSeriesRow != -1) {
        cout << "Номер строки с самой длинной серией одинаковых элементов: " << maxSeriesRow + 1 << endl;
    }
    else {
        cout << "В матрице нет одинаковых элементов.\n";
    }

    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}
