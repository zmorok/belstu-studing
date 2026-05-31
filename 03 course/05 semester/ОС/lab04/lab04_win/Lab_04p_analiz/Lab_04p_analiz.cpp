#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_PRIMES_PER_THREAD 1024
#define MAX_GLOBAL_PRIMES     8192

int primes[MAX_GLOBAL_PRIMES];
int primes_count = 0;

DWORD tlsIndex;

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++) if (n % i == 0) return false;
    return true;
}

DWORD WINAPI L4Primes(LPVOID lpParam)
{
    int* range = (int*)lpParam;
    int start = range[0];
    int end = range[1];

    int* tlsBuffer = (int*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, MAX_PRIMES_PER_THREAD * sizeof(int));

    if (!tlsBuffer) { printf("Ошибка выделения TLS\n"); return 1; }

    TlsSetValue(tlsIndex, tlsBuffer);

    int count = 0;

    for (int n = start; n <= end; n++) if (isPrime(n)) if (count < MAX_PRIMES_PER_THREAD) tlsBuffer[count++] = n;
    for (int i = 0; i < count; i++) if (primes_count < MAX_GLOBAL_PRIMES) primes[primes_count++] = tlsBuffer[i];

    HeapFree(GetProcessHeap(), 0, tlsBuffer);

    return 0;
}

// функция сравнения для qsort
int __cdecl int_cmp(const void* a, const void* b)
{
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

int RunWithThreads(int threadsCount, int low, int high)
{
    primes_count = 0;

    tlsIndex = TlsAlloc();
    if (tlsIndex == TLS_OUT_OF_INDEXES) { printf("Ошибка: TlsAlloc\n"); return 1; }

    HANDLE* threads = (HANDLE*)malloc(sizeof(HANDLE) * threadsCount);
    if (!threads) {
        printf("Ошибка: не удалось выделить память под дескрипторы потоков\n");
        TlsFree(tlsIndex);
        return 1;
    }

    int (*ranges)[2] = (int(*)[2])malloc(sizeof(int) * 2 * threadsCount);
    if (!ranges) {
        printf("Ошибка: не удалось выделить память под диапазоны\n");
        free(threads);
        TlsFree(tlsIndex);
        return 1;
    }

    int totalRange = high - low + 1;
    int baseSize = totalRange / threadsCount;
    int remainder = totalRange % threadsCount;

    int current = low;
    for (int i = 0; i < threadsCount; i++) {
        int partSize = baseSize + (i < remainder ? 1 : 0);
        int start = current;
        int end = start + partSize - 1;
        if (partSize <= 0) { start = 0; end = -1;   /* пустой диапазон */  }

        ranges[i][0] = start;
        ranges[i][1] = end;

        current = end + 1;
    }

    for (int i = 0; i < threadsCount; i++) {
        threads[i] = CreateThread( NULL, 0, L4Primes, (LPVOID)ranges[i], 0, NULL);

        if (!threads[i]) {
            printf("Ошибка создания потока %d, код = %lu\n", i, GetLastError());

            for (int j = 0; j < i; j++) {
                WaitForSingleObject(threads[j], INFINITE);
                CloseHandle(threads[j]);
            }

            free(threads);
            free(ranges);
            TlsFree(tlsIndex);
            return 1;
        }
    }

    DWORD waitRes = WaitForMultipleObjects(threadsCount, threads, TRUE, INFINITE);
    if (waitRes == WAIT_FAILED) {
        printf("Ошибка WaitForMultipleObjects, код = %lu\n", GetLastError());
    }

    for (int i = 0; i < threadsCount; i++)
        CloseHandle(threads[i]);

    free(threads);
    free(ranges);

    TlsFree(tlsIndex);

    return 0;
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
    if (*endptr != '\0') { printf("Ошибка: некорректный нижний порог диапазона\n"); return 1; }
    long highL = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0') { printf("Ошибка: некорректный верхний порог диапазона\n"); return 1; }
    if (lowL > highL) { printf("Ошибка: нижний порог больше верхнего\n"); return 1; }

    int low = (int)lowL;
    int high = (int)highL;

    // число логических процессоров (для лабораторной можно считать за N)
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    int cores = (int)si.dwNumberOfProcessors;

    int maxThreads = 2 * cores;

    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

    printf("Диапазон: [%d; %d], логических процессоров: %d, потоки: 1..%d (x2)\n", low, high, cores, maxThreads);
    printf("Threads\tTime_ms\tPrimesCount\n");

    for (int threadsCount = 1; threadsCount <= maxThreads; threadsCount *= 2) {
        LARGE_INTEGER t1, t2;
        QueryPerformanceCounter(&t1);

        int rc = RunWithThreads(threadsCount, low, high);
        if (rc != 0) {
            printf("Ошибка при запуске с %d потоками\n", threadsCount);
            continue;
        }

        QueryPerformanceCounter(&t2);

        double elapsed_ms = (t2.QuadPart - t1.QuadPart) * 1000.0 / freq.QuadPart;

        qsort(primes, primes_count, sizeof(int), int_cmp);

        printf("%d\t%.3f\t%d\n", threadsCount, elapsed_ms, primes_count);
    }

    return 0;
}