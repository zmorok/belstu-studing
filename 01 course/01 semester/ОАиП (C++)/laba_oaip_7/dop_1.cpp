#include <iostream> 
#include <cmath>
#include <locale.h>

bool isPerfectSquare(int num) {
    int sqrtNum = sqrt(num);
    return (sqrtNum * sqrtNum == num);
}

bool isPrime(int num) {
    if (num <= 1) {
        return false;
    }
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

int main() { 
    setlocale(LC_CTYPE, "Russian");
    using namespace std;
    for (int i = 100; i <= 999; i++) {
        if (isPerfectSquare(i) && isPrime(sqrt(i))) {
            cout << "Найдено число: " << i << ". Квадратный корень " << sqrt(i) << " является простым числомю" << endl;
         }
    }
}