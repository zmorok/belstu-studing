#include <iostream>
#include <cstdarg>
using namespace std;

static int mn(int num, ...)     {

    int max = INT_MIN;

    va_list args;  // Объявляем объект типа va_list для работы с переменным числом аргументов
    va_start(args, num);  // Инициализируем объект va_list, указывая на первый аргумент

    for (int i = 0; i < num; i++) {
        int current = va_arg(args, int);  // Получаем очередной аргумент типа int
        if (current > max) {
            max = current;
        }
    }

    va_end(args);  // Завершаем работу с объектом va_list

    return max;
}

int main() {
    setlocale(0, "");
    int result = mn(5, 10, 7, 25, 3, 15);
    cout << "Максимальное число: " << result << endl;

    return 0;
}
