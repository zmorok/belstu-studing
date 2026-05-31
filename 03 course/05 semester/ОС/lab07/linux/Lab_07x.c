#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

static int isPrime(unsigned long long number)
{
    if (number < 2ULL) return 0;
    if (number == 2ULL) return 1;
    if ((number % 2ULL) == 0ULL) return 0;

    unsigned long long limit = (unsigned long long)sqrt((double)number);
    for (unsigned long long i = 3ULL; i <= limit; i += 2ULL)
        if ((number % i) == 0ULL) return 0;

    return 1;
}

static long long ms_since(const struct timespec* start, const struct timespec* now)
{
    long long sec  = (long long)(now->tv_sec - start->tv_sec);
    long long nsec = (long long)(now->tv_nsec - start->tv_nsec);
    return sec * 1000LL + nsec / 1000000LL;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./Lab_07x <seconds>\n");
        return 1;
    }

    double durationSeconds = atof(argv[1]);
    if (durationSeconds <= 0.0) durationSeconds = 1.1;

    long long durationMs = (long long)(durationSeconds * 1000.0);

    struct timespec start, now;
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0)
    {
        perror("clock_gettime(CLOCK_MONOTONIC)");
        return 1;
    }

    unsigned long long currentNumber = 2ULL;
    unsigned long long primesFound = 0ULL;

    while (1)
    {
        if (clock_gettime(CLOCK_MONOTONIC, &now) != 0)
        {
            perror("clock_gettime(CLOCK_MONOTONIC)");
            return 1;
        }

        if (ms_since(&start, &now) >= durationMs)
            break;

        if (isPrime(currentNumber))
        {
            primesFound++;
            printf("%llu: %llu\n", primesFound, currentNumber);
        }

        currentNumber++;
    }

    double elapsedSeconds = (double)ms_since(&start, &now) / 1000.0;

    printf("\nВсего простых чисел: %llu\n", primesFound);
    printf("Время выполнения: %.3f секунд\n", elapsedSeconds);

    return 0;
}