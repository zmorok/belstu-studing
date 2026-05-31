#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int threadCount = 0;    // количество потоков
int matrixCount = 0;    // количество матриц
int M = 0;              // размер матрицы MxM
int matrixSize = 0;     // M*M

int* matrix = NULL;             // массив размера g_numMatrices * g_matrixSize
volatile LONG* result = NULL;   // итоговая матрица суммы

SYNCHRONIZATION_BARRIER barrier;

typedef struct ThreadParams { int threadIndex; } ThreadParams;

static int random_in_range(int min, int max)
{
    int diff = max - min + 1;
    return min + rand() % diff;
}

static DWORD WINAPI WorkerThread(LPVOID lpParam)
{
    ThreadParams* p = (ThreadParams*)lpParam;
    int tid = p->threadIndex;

    int matricesPerThread = matrixCount / threadCount;
    int extra = matrixCount % threadCount;

    int startMat, endMat;

    // Раздаём «лишние» матрицы первым extra потокам
    if (tid < extra) {
        startMat = tid * (matricesPerThread + 1);
        endMat = startMat + (matricesPerThread + 1);
    }
    else {
        startMat = tid * matricesPerThread + extra;
        endMat = startMat + matricesPerThread;
    }

    if (startMat > matrixCount) startMat = endMat = matrixCount;

    // частичная сумма для этого потока
    int* partial = (int*)calloc(matrixSize, sizeof(int));
    if (!partial) { fprintf(stderr, "Thread %d: failed to allocate partial sum\n", tid); return 1; }

    // расчёт частичных сумм каждым потоком
    for (int m = startMat; m < endMat; ++m) {
        int base = m * matrixSize;
        for (int idx = 0; idx < matrixSize; ++idx) partial[idx] += matrix[base + idx];
    }

    EnterSynchronizationBarrier(&barrier, SYNCHRONIZATION_BARRIER_FLAGS_BLOCK_ONLY);

    // сведение частичных сумм в одну средствами атомарных операций
    for (int idx = 0; idx < matrixSize; ++idx) {
        int val = partial[idx];
        if (val != 0)  InterlockedAdd(&result[idx], val); // атомарно добавляется к итоговой матрице
    }

    EnterSynchronizationBarrier(&barrier, SYNCHRONIZATION_BARRIER_FLAGS_BLOCK_ONLY);

    free(partial);
    return 0;
}

static void print_result_matrix(void)
{
    printf("Итоговая матрица суммы (%dx%d):\n", M, M);
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            int idx = i * M + j;
            printf("%7ld ", (long)result[idx]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[])
{
    if (argc != 4) {
        fprintf(stderr,
            "Использование:\n"
            "%s [Кол-во потоков] [Кол-во матриц] [Размер матриц]\n"
            "Пример: %s 4 10 5\n",
            argv[0], argv[0]);
        return 1;
    }

    threadCount = atoi(argv[1]);
    matrixCount = atoi(argv[2]);
    M = atoi(argv[3]);

    if (threadCount <= 0 || matrixCount <= 0 || M <= 0) {
        fprintf(stderr, "Все аргументы должны быть > 0\n");
        return 2;
    }

    matrixSize = M * M;

    matrix = (int*)malloc(matrixCount * matrixSize * sizeof(int));
    if (!matrix) {
        fprintf(stderr, "Не удалось выделить память под матрицы\n");
        return 3;
    }

    result = (volatile LONG*)calloc(matrixSize, sizeof(LONG));
    if (!result) {
        fprintf(stderr, "Не удалось выделить память под итоговую матрицу\n");
        free(matrix);
        return 4;
    }

    srand((unsigned int)time(NULL));
    for (int m = 0; m < matrixCount; ++m) {
        int base = m * matrixSize;
        for (int idx = 0; idx < matrixSize; ++idx) {
            matrix[base + idx] = random_in_range(-2000, 2000);
        }
    }

    printf("Кол-во потоков: %d\n", threadCount);
    printf("Кол-во матриц: %d\n", matrixCount);
    printf("Размер матриц: %dx%d\n\n", M, M);

    if (!InitializeSynchronizationBarrier(&barrier, threadCount, -1)) {
        fprintf(stderr, "InitializeSynchronizationBarrier failed\n");
        free((void*)result);
        free(matrix);
        return 5;
    }

    HANDLE* threads = (HANDLE*)malloc(threadCount * sizeof(HANDLE));
    ThreadParams* params = (ThreadParams*)malloc(threadCount * sizeof(ThreadParams));
    if (!threads || !params) {
        fprintf(stderr, "Не удалось выделить память под потоки\n");
        if (threads) free(threads);
        if (params)  free(params);
        DeleteSynchronizationBarrier(&barrier);
        free((void*)result);
        free(matrix);
        return 6;
    }

    for (int i = 0; i < threadCount; ++i) {
        params[i].threadIndex = i;
        threads[i] = CreateThread(NULL, NULL, WorkerThread, &params[i], NULL, NULL);
        if (!threads[i]) {
            fprintf(stderr, "CreateThread(%d) failed: %lu\n", i, GetLastError());
            threadCount = i;
            break;
        }
    }

    if (threadCount > 0) WaitForMultipleObjects(threadCount, threads, TRUE, INFINITE);
    
    print_result_matrix();

    for (int i = 0; i < threadCount; ++i)  if (threads[i]) CloseHandle(threads[i]);    
    free(threads); free(params);

    DeleteSynchronizationBarrier(&barrier);
    free((void*)result); free(matrix);

    return 0;
}
