#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>     // sysconf

#define MAX_PRIMES_PER_THREAD 1024
#define MAX_GLOBAL_PRIMES     8192

int primes[MAX_GLOBAL_PRIMES];
int primes_count = 0;

__thread int tlsPrimes[MAX_PRIMES_PER_THREAD];

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

void* L4Primes(void* lpParam)
{
    int* range = (int*)lpParam;
    int start = range[0];
    int end   = range[1];

    int count = 0;
    for (int n = start; n <= end; n++) if (isPrime(n)) if (count < MAX_PRIMES_PER_THREAD) tlsPrimes[count++] = n;
    for (int i = 0; i < count; i++) if (primes_count < MAX_GLOBAL_PRIMES) primes[primes_count++] = tlsPrimes[i];
    return 0;
}

int RunWithThreads(int threadsCount, int low, int high)
{
    primes_count = 0;

    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * threadsCount);
    if (!threads) { printf("Ошибка выделения памяти под потоки\n"); return 1; }

    int (*ranges)[2] = (int(*)[2])malloc(sizeof(int) * 2 * threadsCount);
    if (!ranges) { printf("Ошибка выделения памяти под диапазоны\n"); free(threads); return 1; }

    int totalRange = high - low + 1;
    int baseSize   = totalRange / threadsCount;
    int remainder  = totalRange % threadsCount;

    int current = low;

    for (int i = 0; i < threadsCount; i++) {
        int partSize = baseSize + (i < remainder ? 1 : 0);

        int start = current;
        int end   = start + partSize - 1;

        if (partSize <= 0) { start = 0; end   = -1; /* пустой диапазон */ }

        ranges[i][0] = start;
        ranges[i][1] = end;

        current = end + 1;
    }

    // потоки
    for (int i = 0; i < threadsCount; i++) {
        int rc = pthread_create(&threads[i], NULL, L4Primes, (void*)ranges[i]);
        if (rc != 0) {
            printf("Ошибка создания потока %d, код = %d\n", i, rc);

            for (int j = 0; j < i; j++) pthread_join(threads[j], NULL);

            free(threads);
            free(ranges);
            return 1;
        }
    }

    for (int i = 0; i < threadsCount; i++) pthread_join(threads[i], NULL);

    free(threads);
    free(ranges);

    return 0;
}

double diff_ms(struct timespec a, struct timespec b)
{
    long sec  = b.tv_sec  - a.tv_sec;
    long nsec = b.tv_nsec - a.tv_nsec;
    return (double)sec * 1000.0 + (double)nsec / 1e6;
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Использование для замеров:\n");
        printf("  %s <Нижний_порог> <Верхний_порог>\n", argv[0]);
        printf("Например: %s 1 2097152\n", argv[0]);
        return 1;
    }

    char* endptr = NULL;
    long lowL = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0') { printf("Ошибка: некорректный нижний порог\n"); return 1; }
    long highL = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0') { printf("Ошибка: некорректный верхний порог\n"); return 1; }
    if (lowL > highL) { printf("Ошибка: нижний порог больше верхнего\n"); return 1; }
    int low  = (int)lowL;
    int high = (int)highL;
    long cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (cores < 1) cores = 1;
    int maxThreads = (int)(2 * cores);

    printf("Диапазон: [%d; %d], логических ядер: %ld, потоки: 1..%d (x2)\n", low, high, cores, maxThreads);
    printf("Threads\tTime_ms\tPrimesCount\n");

    struct timespec t1, t2;

    for (int threadsCount = 1; threadsCount <= maxThreads; threadsCount *= 2) {
        clock_gettime(CLOCK_MONOTONIC, &t1);

        int rc = RunWithThreads(threadsCount, low, high);
        if (rc != 0) { printf("Ошибка при запуске с %d потоками\n", threadsCount); continue; }

        clock_gettime(CLOCK_MONOTONIC, &t2);
        double elapsed = diff_ms(t1, t2);
        printf("%d\t%.3f\t%d\n", threadsCount, elapsed, primes_count);
    }

    return 0;
}
