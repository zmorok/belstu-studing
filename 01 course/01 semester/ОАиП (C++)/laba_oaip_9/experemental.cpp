#include <iostream>
#include <cmath>

double f(double x) {
    return exp(x) - 3 - 1 / x;
}

int main() {
    double a = 0.1; // Левый конец интервала
    double b = 2.0; // Правый конец интервала
    double epsilon = 0.0001; // Точность

    double x;

    while ((b - a) >= epsilon) {
        x = (a + b) / 2;
        if (f(x) == 0) {
            break;
        }
        else if (f(x) * f(a) < 0) {
            b = x;
        }
        else {
            a = x;
        }
    }

    std::cout << "Найденный корень: " << x << std::endl;
    
    return 0;
}