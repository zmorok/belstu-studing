#include <iostream>

using namespace std;

// вывод перестановок
static void printPermutations(int* numbers, int n, int left, int right) {
    // достигнут конец массива, печатается перестановка
    if (left == right) { 
        for (int i = 0; i < n; i++) {
            cout << numbers[i] << " ";
        }
        cout << endl;
    }
    else {
        for (int i = left; i <= right; i++) {
            swap(numbers[left], numbers[i]);                // элемент меняется с каждым оставшимся
            printPermutations(numbers, n, left + 1, right); // генерация перестановок для оставшихся элементов
            swap(numbers[left], numbers[i]);                // возвращение элемента для следующей перестановки
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    int n;
    cout << "Введите количество натуральных чисел: ";
    cin >> n;

    if (n <= 0) {
        cerr << "Error." << endl;
        return 1;
    }

    int* numbers = new int[n];
    cout << "Введите натуральные числа через пробел: ";
    for (int i = 0; i < n; ++i) {
        cin >> numbers[i];
    }

    cout << "Перестановки:" << endl;
    printPermutations(numbers, n, 0, n - 1);

    delete[] numbers;

    return 0;
}
