#include <iostream>
using namespace std;

static int summation(char* word) {
    int sum = 0;
    int currentNumber = 0;

    for (char* ptr = word; *ptr; ptr++) {
        char sym = *ptr;
        if (isdigit(sym)) {
            currentNumber = currentNumber * 10 + (sym - '0');
        }
        else {
            sum += currentNumber;
            currentNumber = 0;
        }
    }
    sum += currentNumber; // добавить в сумму последнее число строки, если та заканчивается на число
    return sum;
}

int main() {
    setlocale(0, "");
    const int SIZE = 1000;
    char word[SIZE];
    cout << "Введите вашу строку, состоящую из цифр и чисел: \n";
    cin.getline(word, SIZE);

    int sum = summation(word);

    cout << "Сумма чисел в вашей строке равна " << sum << endl;
}
