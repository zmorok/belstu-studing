#include <iostream>
using namespace std;

int main() {
    setlocale(0, "");
    int rows, cols;
    cout << "Введите количество строк матрицы: ";
    cin >> rows;
    
    cout << "Введите количество столбцов матрицы: ";
    cin >> cols;

    // matrix - двойной указатель
    int** matrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        // Для каждой строки выделяется память
        matrix[i] = (int*)malloc(cols * sizeof(int));
    }

    cout << "Введите элементы матрицы:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << "Элемент [" << i + 1 << "][" << j + 1 << "]: ";
            cin >> matrix[i][j];
        }
    }

    int countNonZeroColumns = 0;
    for (int j = 0; j < cols; j++) {
        bool hasZero = false;
        for (int i = 0; i < rows; i++) {
            if (matrix[i][j] == 0) {
                hasZero = true;
                break;
            }
        }
        if (!hasZero) {
            countNonZeroColumns++;
        }
    }

    
    cout << "Количество столбцов без нулевых элементов: " << countNonZeroColumns << endl;

    
    for (int i = 0; i < rows; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}
