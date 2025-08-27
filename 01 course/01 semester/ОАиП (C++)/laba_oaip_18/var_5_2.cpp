#include <iostream>
#include <cstdarg>
using namespace std;

static int kvadr(int num, ...) {
    
    int counter = 0;

    va_list args;  // Объявляем объект типа va_list для работы с переменным числом аргументов
    va_start(args, num);  // Инициализируем объект va_list, указывая на первый аргумент

    for (int i = 0; i < num; i++) {
        int x = va_arg(args, int);  // Получаем очередной аргумент типа int

        int sqrtX = int(sqrt(x));
        
        if (sqrtX * sqrtX == x)
            counter++;
    }

    va_end(args);  // Завершаем работу с объектом va_list

    return counter;
}

int main() {
    setlocale(0, "");
    int result = kvadr(5, 2, 4, 9, 16, 25);
    cout << "Кол-во точных квадратов: " << result << endl;

    return 0;
}
