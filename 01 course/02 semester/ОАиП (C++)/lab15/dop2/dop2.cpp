#include <iostream>
#include <algorithm>
#include <chrono>
using namespace std;

// поиск минимального элемента (для массива B)
static int findMin(int* arrayB, int sizeB) {
    int min = arrayB[0];
    for (int i = 1; i < sizeB; ++i) { if (arrayB[i] < min) { min = arrayB[i]; } }
    return min;
}

// копирование элементов массива A в массив C, больших минимального элемента массива B
static int* transferElements(int* arrayA, int sizeA, int* arrayB, int sizeB, int minB, int& sizeC) {
    int* arrayC = (int*)malloc(sizeA * sizeof(int));
    sizeC = 0;
    if (arrayC == nullptr) { sizeC = 0; return nullptr; }
    for (int i = 0; i < sizeA; ++i) { if (arrayA[i] > minB) { arrayC[sizeC++] = arrayA[i]; } }
    int* temp = (int*)realloc(arrayC, sizeC * sizeof(int));
    if (temp == nullptr) { free(arrayC); sizeC = 0; return nullptr; }
    arrayC = temp;
    return arrayC;
}

// сортировка выбором по убыванию
static void selectionSortDescending(int* arr, int n) {
    for (int i = 0; i < n - 1; ++i) {
        int maxIndex = i;
        for (int j = i + 1; j < n; ++j) { if (arr[j] > arr[maxIndex]) { maxIndex = j; } }
        if (maxIndex != i) { swap(arr[i], arr[maxIndex]); }
    }
}


int main() {
    setlocale(LC_ALL, "Russian"); srand(time(NULL));

    int sizeA, sizeB;

    cout << endl << "Введите размер массива A: ";
    cin >> sizeA;
    if (sizeA < 1) { cerr << "Неверный размер массива."; return 1; }
    int* arrayA = new int[sizeA];
    cout << "Элементы массива A: ";
    for (int i = 0; i < sizeA; ++i) { arrayA[i] = rand() % 101; cout << arrayA[i] << " "; }

    cout << endl << endl << "Введите размер массива B: ";
    cin >> sizeB;
    if (sizeB < 1) { cerr << "Неверный размер массива."; return 1; }
    int* arrayB = new int[sizeB];
    cout << "Элементы массива B: ";
    for (int i = 0; i < sizeB; ++i) { arrayB[i] = rand() % 101; cout << arrayB[i] << " "; }

    int minB = findMin(arrayB, sizeB);
    cout << endl << endl << "Минимальный элемент массива B: " << minB;

    int sizeC = 0;
    int* arrayC = transferElements(arrayA, sizeA, arrayB, sizeB, minB, sizeC);

    cout << endl << endl << "Массив C до сортировки выбором: ";
    for (int i = 0; i < sizeC; ++i) { cout << arrayC[i] << " "; }

    selectionSortDescending(arrayC, sizeC);

    cout << endl << endl << "Массив C после сортировки выбором: ";
    for (int i = 0; i < sizeC; ++i) { cout << arrayC[i] << " "; } cout << endl;

    delete[] arrayA; delete[] arrayB; delete[] arrayC;
    return 0;
}

/*int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));

    int sizes[] = { 1000, 2000, 3000, 4000, 5000, 10000, 30000, 50000, 100000, 200000, 500000, 1000000, 2000000, 5000000 };

    for (int size : sizes) {
        int* arrayA = new int[size];
        int* arrayB = new int[size];
        // Генерация случайных массивов
        for (int i = 0; i < size; ++i) {
            arrayA[i] = rand() % 5001;
            arrayB[i] = rand() % 5001;
        }

        int minB = findMin(arrayB, size);
        int sizeC = 0;
        int* arrayC = transferElements(arrayA, size, arrayB, size, minB, sizeC);

        auto start_time = chrono::high_resolution_clock::now();
        selectionSortDescending(arrayC, sizeC);
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
        cout << "Cортировка выбором для n = " << size << " заняла " << duration.count() / 1000.0 << " миллисекунд." << endl;

        delete[] arrayA;
        delete[] arrayB;
        delete[] arrayC;
    }

    return 0;
}*/