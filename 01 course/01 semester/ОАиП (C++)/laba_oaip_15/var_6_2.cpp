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
            std::cout << "Элемент [" << i + 1 << "][" << j + 1 << "]: ";
            std::cin >> matrix[i][j];
        }
    }

    // Поиск строки с отрицательным элементом и уменьшение соответствующего столбца вдвое
    bool foundNegativeRow = false;
    int negativeRow = -1;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] < 0) {
                foundNegativeRow = true;
                negativeRow = i;

                for (int k = 0; k < rows; k++) {
                    matrix[k][j] /= j+1;
                }
            }
        }

        if (foundNegativeRow) {
            break;
        }
    }

    if (foundNegativeRow) {
        cout << "Номер строки с отрицательным элементом: " << negativeRow + 1 << endl;
    }
    else {
        cout << "В матрице нет строки с отрицательным элементом.\n";
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << "[ " << matrix[i][j] << " ]";
        }
        cout << endl;
    }

    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}
