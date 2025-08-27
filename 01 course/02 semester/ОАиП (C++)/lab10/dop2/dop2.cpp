#include <iostream>
#include <cstdlib>

using namespace std;

const int MAX_COUNTRIES = 100;
int matrix[MAX_COUNTRIES][MAX_COUNTRIES];

// обход матрицы и разделения стран на группы
static void searching(int matrix[][MAX_COUNTRIES], int numCountries, int country, int group, int groups[]) {
    
    // Помечаем текущую страну принадлежащей к текущей группе
    groups[country] = group; 

    
    for (int neighbor = 0; neighbor < numCountries; ++neighbor) {

        // Если смежная страна ещё не посещена и имеет общую границу с текущей страной
        if (groups[neighbor] == -1 && matrix[country][neighbor] == 1) { 
            searching(matrix, numCountries, neighbor, 1 - group, groups);
        }

    }
}

// разделение стран на две группы
static void divideCountries(int matrix[][MAX_COUNTRIES], int numCountries) {
    int groups[MAX_COUNTRIES];
    for (int i = 0; i < numCountries; ++i) {
        groups[i] = -1;
    }

    // обход матрицы, начиная с произвольной страны (0) и помечаем её в первой группе (0)
    searching(matrix, numCountries, 0, 0, groups);

    // результат разделения стран на группы
    cout << "Группа 1: ";
    for (int i = 0; i < numCountries; ++i) {
        if (groups[i] == 0) {
            cout << ++i << " ";
        }
    }
    cout << endl;

    cout << "Группа 2: ";
    for (int i = 0; i < numCountries; ++i) {
        if (groups[i] == 1) {
            cout << ++i << " ";
        }
    }
    cout << endl;

    // вывод матрицы смежности
    cout << "Матрица смежности:" << endl;
    for (int i = 0; i < numCountries; ++i) {
        for (int j = 0; j < numCountries; ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");    
    int numCountries = 6; 

    srand(time(NULL));
    for (int i = 0; i < numCountries; i++) {
        for (int j = 0; j < numCountries; j++) {
            matrix[i][j] = rand() % 2; 
        }
    }

    divideCountries(matrix, numCountries);

    return 0;
}
