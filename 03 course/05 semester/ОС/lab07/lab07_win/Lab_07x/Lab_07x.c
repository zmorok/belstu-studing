#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int isPrime(unsigned long long number)
{
    if (number < 2ULL) return 0;
    if (number == 2ULL) return 1;
    if ((number % 2ULL) == 0ULL) return 0;

    unsigned long long limit = (unsigned long long)sqrt((double)number);
    for (unsigned long long i = 3ULL; i <= limit; i += 2ULL) if ((number % i) == 0ULL) return 0;
    
    return 1;
}

int main(int argc, char* argv[])
{
    if (argc < 2) { printf("Usage: Lab-07x.exe <seconds>\n"); return 1; }

    double durationSeconds = atof(argv[1]);
    if (durationSeconds <= 0.0) durationSeconds = 1.1;
    ULONGLONG durationMs = (ULONGLONG)(durationSeconds * 1000.0);
    ULONGLONG startTime = GetTickCount64();

    unsigned long long currentNumber = 2ULL;
    unsigned long long primesFound = 0ULL;

    while ((GetTickCount64() - startTime) < durationMs)
    {
        if (isPrime(currentNumber))
        {
            primesFound++;
            printf("%llu: %llu\n", primesFound, currentNumber);
        }
        currentNumber++;
    }

    ULONGLONG endTime = GetTickCount64();
    double elapsedSeconds = (double)(endTime - startTime) / 1000.0;

    printf("\nВсего простых чисел: %llu\n", primesFound);
    printf("Время выполнения: %.3f секунд\n", elapsedSeconds);

    return 0;
}
