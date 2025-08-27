#include <iostream>
#include <cstdarg>
using namespace std;

static int calculateSum(int num, ...) {

    va_list args;
    va_start(args, num);

    int sum = 0;
    int sign = 1;

    for (int i = 0; i < num; ++i) {
        int current = va_arg(args, int);

        sum += sign * current;
        sign = -sign;  // Меняем знак перед следующим слагаемым
    }

    va_end(args);

    return sum;
}

int main() {
    setlocale(0, "");
    int result = calculateSum(5, 10, 7, 25, 3, 15);
    cout << "Сумма чисел: " << result << endl;

    return 0;
}
