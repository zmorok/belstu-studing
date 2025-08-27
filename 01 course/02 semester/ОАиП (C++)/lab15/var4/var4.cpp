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

// построение пирамиды для сортировки
static void heapify(int* array, int size, int root) {
    int largest = root;
    int left = 2 * root + 1;
    int right = 2 * root + 2;
    if (left < size && array[left] < array[largest]) { largest = left; }
    if (right < size && array[right] < array[largest]) { largest = right; }
    if (largest != root) { swap(array[root], array[largest]); heapify(array, size, largest); }
}

// пирамидальная сортировка по убыванию
static void pyramidSort(int* array, int size) {
    for (int i = size - 1; i >= 0; --i) { heapify(array, size, i); }
    for (int i = size - 1; i > 0; --i) { swap(array[0], array[i]); heapify(array, i, 0);}
}

// слияние двух подмассивов отсортированных по убыванию
static void merge(int* arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int* L = new int[n1];
    int* R = new int[n2];
    for (int i = 0; i < n1; i++) { L[i] = arr[l + i]; }
    for (int j = 0; j < n2; j++) { R[j] = arr[m + 1 + j]; }
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] >= R[j]) { arr[k++] = L[i++]; }
        else { arr[k++] = R[j++]; }
    }
    // копирование оставшихся элементов из подмассивов, если они есть
    while (i < n1) { arr[k++] = L[i++]; }
    while (j < n2) { arr[k++] = R[j++]; }
    delete[] L;
    delete[] R;
}

// рекурсивно сортировка слиянием
static void mergeSort(int* arr, int l, int r) {
    if (l >= r) { return; }
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

// сортировка слиянием по убыванию
static void mergeSortDescending(int* array, int size) { mergeSort(array, 0, size - 1); }

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

    int sizeC1 = 0, sizeC2 = 0;
    int* arrayC1 = transferElements(arrayA, sizeA, arrayB, sizeB, minB, sizeC1);
    int* arrayC2 = transferElements(arrayA, sizeA, arrayB, sizeB, minB, sizeC2);

    cout << endl << endl << "Массив C1 до пирамидальной сортировки: ";
    for (int i = 0; i < sizeC1; ++i) { cout << arrayC1[i] << " "; }
    cout << endl << "Массив C2 до сортировки слиянием: ";
    for (int i = 0; i < sizeC2; ++i) { cout << arrayC2[i] << " "; }

    pyramidSort(arrayC1, sizeC1);
    mergeSortDescending(arrayC2, sizeC2);

    cout << endl << endl << "Массив C1 после пирамидальной сортировки: ";
    for (int i = 0; i < sizeC1; ++i) { cout << arrayC1[i] << " "; }
    cout << endl << "Массив C2 после сортировки слиянием: ";
    for (int i = 0; i < sizeC2; ++i) { cout << arrayC2[i] << " "; } cout << endl;

    delete[] arrayA; delete[] arrayB; delete[] arrayC1; delete[] arrayC2;
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
        int sizeC1 = 0, sizeC2 = 0;
        int* arrayC1 = transferElements(arrayA, size, arrayB, size, minB, sizeC1);
        int* arrayC2 = transferElements(arrayA, size, arrayB, size, minB, sizeC2);

        auto start_time = chrono::high_resolution_clock::now();
        pyramidSort(arrayC1, sizeC1);
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
        cout << "Пирамидальная сортировка для n = " << size << " заняла " << duration.count() / 1000.0 << " миллисекунд." << endl;

        start_time = chrono::high_resolution_clock::now();
        mergeSortDescending(arrayC2, sizeC2);
        end_time = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
        cout << "Сортировка слиянием для n = " << size << " заняла " << duration.count() / 1000.0 << " миллисекунд." << endl;

        delete[] arrayA;
        delete[] arrayB;
        delete[] arrayC1;
        delete[] arrayC2;
    }

    return 0;
}*/