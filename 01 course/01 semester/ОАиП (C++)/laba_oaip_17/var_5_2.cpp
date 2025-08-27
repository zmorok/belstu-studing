#include <iostream>
using namespace std;

static void multiplyMatrices(int** firstMatrix, int** secondMatrix, int** result, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = 0;
            for (int k = 0; k < m; k++) {
                result[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
            }
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

    if (rows <= 0 || cols <= 0) {
        cerr << endl << "Fatal error!" << endl;
        return 1;
    }

    int** firstMatrix = new int*[rows];
    for (int i = 0; i < rows; i++)
        firstMatrix[i] = new int[cols];

    int** secondMatrix = new int*[cols];
    for (int i = 0; i < cols; i++)
        secondMatrix[i] = new int[rows];

    int** result = new int*[rows];
    for (int i = 0; i < rows; i++)
        result[i] = new int[rows];


    srand(time(NULL));
    cout << endl << "Первая матрица:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            firstMatrix[i][j] = rand() % 2 == 1 ? rand() % 10 : (rand() % 10) * -1;
            cout << "[ " << firstMatrix[i][j] << " ]";
        }
        cout << endl;
    }

    cout << endl << "Вторая матрица:" << endl;
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            secondMatrix[i][j] = rand() % 2 == 1 ? rand() % 10 : (rand() % 10) * -1;
            cout << "[ " << secondMatrix[i][j] << " ]";
        }
        cout << endl;
    }

    multiplyMatrices(firstMatrix, secondMatrix, result, rows, cols);

    cout << "Результат умножения матриц:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            cout << "[ " << result[i][j] << " ]";
        }
        cout << endl;
    }
}