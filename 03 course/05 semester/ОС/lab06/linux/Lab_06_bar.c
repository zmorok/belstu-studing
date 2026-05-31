#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int threadCount  = 0;  // количество потоков
int matrixCount  = 0;  // количество матриц
int M            = 0;  // размер матрицы MxM
int matrixSize   = 0;  // M*M

int        *matrix = NULL;  // все матрицы подряд
long long  *result = NULL;  // итоговая матрица суммы

pthread_barrier_t barrier;
pthread_mutex_t result_mutex;

typedef struct ThreadParams { int threadIndex; } ThreadParams;

static int random_in_range(int min, int max)
{
    int diff = max - min + 1;
    return min + rand() % diff;
}

// функция потока
static void* WorkerThread(void* arg)
{
    ThreadParams* p = (ThreadParams*)arg;
    int tid = p->threadIndex;

    int matricesPerThread = matrixCount / threadCount;
    int extra             = matrixCount % threadCount;

    int startMat, endMat;

    if (tid < extra) {
        startMat = tid * (matricesPerThread + 1);
        endMat   = startMat + (matricesPerThread + 1);
    } else {
        startMat = tid * matricesPerThread + extra;
        endMat   = startMat + matricesPerThread;
    }

    if (startMat > matrixCount) startMat = endMat = matrixCount;

    // локальная частичная сумма для этого потока
    int* partial = (int*)calloc(matrixSize, sizeof(int));
    if (!partial) {
        fprintf(stderr, "Thread %d: failed to allocate partial sum\n", tid);
        return NULL;
    }

    // расчёт частичных сумм
    for (int m = startMat; m < endMat; ++m) {
        int base = m * matrixSize;
        for (int idx = 0; idx < matrixSize; ++idx) partial[idx] += matrix[base + idx];
    }

    pthread_barrier_wait(&barrier);

    pthread_mutex_lock(&result_mutex);
    for (int idx = 0; idx < matrixSize; ++idx) result[idx] += partial[idx];
    pthread_mutex_unlock(&result_mutex);

    pthread_barrier_wait(&barrier);

    free(partial);
    return NULL;
}

static void print_result_matrix(void)
{
    printf("Итоговая матрица суммы (%dx%d):\n", M, M);
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            int idx = i * M + j;
            printf("%7lld ", result[idx]);
        }
        printf("\n");
    }
}

static double timespec_diff_sec(const struct timespec* t2, const struct timespec* t1)
{
    time_t ds = t2->tv_sec - t1->tv_sec;
    long   dn = t2->tv_nsec - t1->tv_nsec;
    return (double)ds + (double)dn / 1e9;
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
    M           = atoi(argv[3]);

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

    result = (long long*)calloc(matrixSize, sizeof(long long));
    if (!result) {
        fprintf(stderr, "Не удалось выделить память под итоговую матрицу\n");
        free(matrix);
        return 4;
    }

    srand((unsigned int)time(NULL));
    for (int m = 0; m < matrixCount; ++m) {
        int base = m * matrixSize;
        for (int idx = 0; idx < matrixSize; ++idx) matrix[base + idx] = random_in_range(-2000, 2000);
    }

    printf("Кол-во потоков: %d\n", threadCount);
    printf("Кол-во матриц: %d\n", matrixCount);
    printf("Размер матриц: %dx%d\n\n", M, M);

    if (pthread_mutex_init(&result_mutex, NULL) != 0) {
        perror("pthread_mutex_init");
        free(result);
        free(matrix);
        return 5;
    }

    if (pthread_barrier_init(&barrier, NULL, threadCount) != 0) {
        perror("pthread_barrier_init");
        pthread_mutex_destroy(&result_mutex);
        free(result);
        free(matrix);
        return 6;
    }

    pthread_t*    threads = (pthread_t*)malloc(threadCount * sizeof(pthread_t));
    ThreadParams* params  = (ThreadParams*)malloc(threadCount * sizeof(ThreadParams));
    if (!threads || !params) {
        fprintf(stderr, "Не удалось выделить память под потоки\n");
        free(threads);
        free(params);
        pthread_barrier_destroy(&barrier);
        pthread_mutex_destroy(&result_mutex);
        free(result);
        free(matrix);
        return 7;
    }

    struct timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);

    // потоки
    for (int i = 0; i < threadCount; ++i) {
        params[i].threadIndex = i;
        if (pthread_create(&threads[i], NULL, WorkerThread, &params[i]) != 0) {
            perror("pthread_create");
            threadCount = i;
            break;
        }
    }

    for (int i = 0; i < threadCount; ++i) pthread_join(threads[i], NULL);
    
    clock_gettime(CLOCK_MONOTONIC, &t2);
    double elapsed = timespec_diff_sec(&t2, &t1);

    print_result_matrix();
    printf("\nОбщее время вычисления: %.6f сек\n", elapsed);

    free(threads);
    free(params);

    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&result_mutex);

    free(result);
    free(matrix);

    return 0;
}
