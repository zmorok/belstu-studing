#include <iostream>

using namespace std;

static void findMaxOfMinInMatrix(int** matrix, int rows, int cols, int& maxOfMinValue, int& maxOfMinRow) {
    maxOfMinValue = INT_MIN; // константа, представляющая наименьшее значение для int () -2147483648
    maxOfMinRow = 0;         // с maxOfMinValue = matrix[0][0]; не работает
    
    for (int i = 0; i < rows; i++) {
        int minInRow = matrix[i][0];

        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] < minInRow) {
                minInRow = matrix[i][j];
            }
        }

        if (minInRow > maxOfMinValue) {
            maxOfMinValue = minInRow;
            maxOfMinRow = i;
        }
    }
}

int main() {
    setlocale(0, "");

    int rows, cols;

    cout << "Введите количество строк: ";
    cin >> rows;

    cout << "Введите количество столбцов: ";
    cin >> cols;

    int** matrix = new int* [rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }

    srand(time(NULL));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 101;
        }
    }

    cout << "Матрица: " << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << "[ " << matrix[i][j] << " ]";
        }
        cout << endl;
    }

    int maxOfMinValue, maxOfMinRow;


    findMaxOfMinInMatrix(matrix, rows, cols, maxOfMinValue, maxOfMinRow);

    cout << "Максимальный минимальный элемент находится в строке: " << maxOfMinRow + 1 << endl;
    cout << "Значение: " << maxOfMinValue << endl;

    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

}
