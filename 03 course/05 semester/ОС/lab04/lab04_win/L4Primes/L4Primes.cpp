#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_PRIMES_PER_THREAD 1024
#define MAX_GLOBAL_PRIMES     8192

int primes[MAX_GLOBAL_PRIMES];
int primes_count = 0;

DWORD tlsIndex; // индекс TLS

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

int main()
{
    tlsIndex = TlsAlloc();
    if (tlsIndex == TLS_OUT_OF_INDEXES) { printf("ERROR: TlsAlloc failed\n"); return 1; }

    int ranges[2][2] = { { 2, 5000 }, { 5001, 10000 } };

    HANDLE threads[2];
    for (int i = 0; i < 2; i++) {
        threads[i] = CreateThread(NULL, 0, L4Primes, (LPVOID)ranges[i], 0, NULL);
        if (!threads[i]) { printf("Ошибка создания потока %d\n", i); return 1; }
    }

    WaitForMultipleObjects(2, threads, TRUE, INFINITE);
    for (int i = 0; i < 2; i++) CloseHandle(threads[i]);

    TlsFree(tlsIndex);

    printf("Глобальный массив содержит %d простых чисел:\n", primes_count);
    for (int i = 0; i < primes_count; i++) printf("%d ", primes[i]);

    printf("\n");
    return 0;
}