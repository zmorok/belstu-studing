#include <iostream>
using namespace std;

int main() {
    setlocale(0, "");
    const int rows = 100, columns = 100;
    int n, m;
    cout << "Введите количество строк матрицы: ";
    cin >> n;
    cout << "Введите количество столбцов матрицы: ";
    cin >> m;

    int matrix[rows][columns];
    cout << "Введите элементы матрицы:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> matrix[i][j];
        }
    }

    bool foundPositive = false; // Флаг для отслеживания наличия строки с положительным элементом
    int positiveRow = -1; // Номер строки с положительным элементом

    // Поиск строки с положительным элементом
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (matrix[i][j] > 0) {
                foundPositive = true;
                positiveRow = i;
                break;
            }
        }
        if (foundPositive) {
            break;
        }
    }

    // Проверка наличия строки с положительным элементом
    if (foundPositive) {
        cout << "Есть строка с положительным элементом. Номер строки: " << positiveRow + 1 << endl;

        // Изменение знаков элементов предыдущей строки
        if (positiveRow > 0) {
            for (int j = 0; j < m; ++j) {
                matrix[positiveRow - 1][j] = -matrix[positiveRow - 1][j];
            }

            // Вывод обновленной матрицы
            cout << "Матрица с измененными знаками элементов предыдущей строки:\n";
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    cout << matrix[i][j] << " ";
                }
                cout << endl;
            }
        }
        else {
            cout << "Строка с положительным элементом - первая строка, изменение знаков невозможно.\n";
        }
    }
    else {
    }
}
