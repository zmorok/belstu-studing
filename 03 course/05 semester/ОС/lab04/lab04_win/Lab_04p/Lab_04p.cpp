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

int main(int argc, char* argv[])
{
    if (argc != 4) { printf("Использование:\n"); printf("  %s <Количество_потоков> <Нижний_порог> <Верхний_порог>\n", argv[0]); return 1; }

    char* endptr = NULL;
    long threadsCountL = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || threadsCountL <= 0) { printf("Ошибка: некорректное количество потоков\n"); return 1; }
    long lowL = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0') { printf("Ошибка: некорректный нижний порог диапазона\n"); return 1; }
    long highL = strtol(argv[3], &endptr, 10);
    if (*endptr != '\0') { printf("Ошибка: некорректный верхний порог диапазона\n"); return 1; }
    if (lowL > highL) { printf("Ошибка: нижний порог больше верхнего\n"); return 1; }
    if (threadsCountL > MAXIMUM_WAIT_OBJECTS) { printf("Ошибка: количество потоков превышает %d (ограничение WaitForMultipleObjects)\n", MAXIMUM_WAIT_OBJECTS); return 1; }

    int threadsCount = (int)threadsCountL;
    int low = (int)lowL;
    int high = (int)highL;

    tlsIndex = TlsAlloc();
    if (tlsIndex == TLS_OUT_OF_INDEXES) { printf("Ошибка: TlsAlloc\n"); return 1; }

    // массив дескрипторов потоков
    HANDLE* threads = (HANDLE*)malloc(sizeof(HANDLE) * threadsCount);
    if (!threads) {
        printf("Ошибка: не удалось выделить память под дескрипторы потоков\n");
        TlsFree(tlsIndex);
        return 1;
    }

    // массив диапазонов для каждого потока
    int (*ranges)[2] = (int(*)[2])malloc(sizeof(int) * 2 * threadsCount);
    if (!ranges) {
        printf("Ошибка: не удалось выделить память под диапазоны\n");
        free(threads);
        TlsFree(tlsIndex);
        return 1;
    }

    // деление диапазонов на равные части
    int totalRange = high - low + 1;
    int baseSize = totalRange / threadsCount;
    int remainder = totalRange % threadsCount;

    int current = low;
    for (int i = 0; i < threadsCount; i++) {
        int partSize = baseSize + (i < remainder ? 1 : 0);
        int start = current;
        int end = start + partSize - 1;
        if (partSize <= 0) { start = 0; end = -1; /* пустой диапазон */ }

        ranges[i][0] = start;
        ranges[i][1] = end;

        current = end + 1;
    }

    // потоки
    for (int i = 0; i < threadsCount; i++) {
        threads[i] = CreateThread(NULL, 0, L4Primes, (LPVOID)ranges[i], 0, NULL);

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

    for (int i = 0; i < threadsCount; i++) CloseHandle(threads[i]);

    free(threads);
    free(ranges);

    TlsFree(tlsIndex);

    if (primes_count > 0) qsort(primes, primes_count, sizeof(int), int_cmp);

    printf("Глобальный массив содержит %d простых чисел:\n", primes_count);
    for (int i = 0; i < primes_count; i++) printf("%d ", primes[i]);

    printf("\n");

    return 0;
}