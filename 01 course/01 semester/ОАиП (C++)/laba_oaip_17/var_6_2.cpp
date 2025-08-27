#include <iostream>
using namespace std;

static void checkAndReplace(int** matrix, int rows, int cols) {
    bool allRowsContainZero = true;
    for (int i = 0; i < rows; i++) {
        bool rowContainsZero = false;
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == 0) {
                rowContainsZero = true;
                break;
            }
        }
        if (!rowContainsZero) {
            allRowsContainZero = false;
            break;
        }
    }

    if (allRowsContainZero) {
        cout << endl << "Все строки содержат хотя бы один нулевой элемент." << endl;
    }
    else {
        cout << endl << "Не все строки содержат хотя бы один нулевой элемент." << endl;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (matrix[i][j] < 0) {
                    matrix[i][j] = 0;
                }
            }
        }

        cout << endl << "Матрица после замены отрицательных элементов на нули:" << endl;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << "[ " << matrix[i][j] << " ]";
            }
            cout << endl;
        }
    }
}

int main() {
    setlocale(0, "");
    int rows, cols;

    cout << "Введите количество строк (rows): ";
    cin >> rows;
    cout << "Введите количество столбцов (cols): ";
    cin >> cols;

    int** matrix = new int*[rows];
    for (int i = 0; i < rows; i++)
        matrix[i] = new int[cols];

    cout << "Исходная матрица:\n";
    srand(time(NULL));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = rand() % 2 == 1 ? rand() % 101 : (rand() % 101) * -1;
            matrix[i][j] = rand() % 2 == 0 ? 0 : matrix[i][j];
            cout << "[ " << matrix[i][j] << " ]";
        }
        cout << endl;
    }

    checkAndReplace(matrix, rows, cols);

    delete[] matrix;
}
