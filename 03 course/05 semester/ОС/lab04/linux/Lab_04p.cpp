#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PRIMES_PER_THREAD 1024
#define MAX_GLOBAL_PRIMES     8192

int primes[MAX_GLOBAL_PRIMES];
int primes_count = 0;

static pthread_key_t tlsKey;
static pthread_once_t tlsOnce = PTHREAD_ONCE_INIT;

static void tls_destructor(void *ptr) { free(ptr); }

static void tls_init_key(void) {
    if (pthread_key_create(&tlsKey, tls_destructor) != 0) { fprintf(stderr, "pthread_key_create failed\n"); exit(1); }
}

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++) if (n % i == 0) return false;
    return true;
}

void* L4Primes(void* lpParam)
{
    int* range = (int*)lpParam;
    int start = range[0];
    int end   = range[1];

    pthread_once(&tlsOnce, tls_init_key);

    // буфер для текущего потока
    int* tlsBuffer = (int*)pthread_getspecific(tlsKey);
    if (!tlsBuffer) {
        tlsBuffer = (int*)calloc(MAX_PRIMES_PER_THREAD, sizeof(int));
        if (!tlsBuffer) { fprintf(stderr, "Ошибка выделения TLS\n");  return (void*)1; }
        if (pthread_setspecific(tlsKey, tlsBuffer) != 0) { fprintf(stderr, "pthread_setspecific failed\n"); free(tlsBuffer); return (void*)1; }
    }

    int count = 0;

    for (int n = start; n <= end; n++) if (isPrime(n)) if (count < MAX_PRIMES_PER_THREAD) tlsBuffer[count++] = n;
    for (int i = 0; i < count; i++) if (primes_count < MAX_GLOBAL_PRIMES) primes[primes_count++] = tlsBuffer[i];

    return 0;
}

// функция сравнения для qsort
int int_cmp(const void* a, const void* b)
{
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

int main(int argc, char* argv[])
{
    if (argc != 4) {
        printf("Использование:\n");
        printf("  %s <Количество_потоков> <Нижний_порог> <Верхний_порог>\n", argv[0]);
        return 1;
    }

    char* endptr = NULL;
    long threadsCountL = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || threadsCountL <= 0) { printf("Ошибка: некорректное количество потоков\n"); return 1; }
    long lowL = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0') { printf("Ошибка: некорректный нижний порог диапазона\n"); return 1; }
    long highL = strtol(argv[3], &endptr, 10);
    if (*endptr != '\0') { printf("Ошибка: некорректный верхний порог диапазона\n"); return 1; }
    if (lowL > highL) { printf("Ошибка: нижний порог больше верхнего\n"); return 1; }

    int threadsCount = (int)threadsCountL;
    int low  = (int)lowL;
    int high = (int)highL;

    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * threadsCount);
    if (!threads) { printf("Ошибка: не удалось выделить память под дескрипторы потоков\n"); return 1; }

    int (*ranges)[2] = (int(*)[2])malloc(sizeof(int) * 2 * threadsCount);
    if (!ranges) { printf("Ошибка: не удалось выделить память под диапазоны\n"); free(threads); return 1; }

    // диапазоны
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
            fprintf(stderr, "Ошибка создания потока %d, код = %d\n", i, rc);

            for (int j = 0; j < i; j++) pthread_join(threads[j], NULL);

            free(threads);
            free(ranges);
            return 1;
        }
    }

    for (int i = 0; i < threadsCount; i++)  pthread_join(threads[i], NULL);

    free(threads);
    free(ranges);

    if (primes_count > 0) qsort(primes, primes_count, sizeof(int), int_cmp);

    printf("Глобальный массив содержит %d простых чисел:\n", primes_count);
    for (int i = 0; i < primes_count; i++) printf("%d ", primes[i]);

    printf("\n");

    return 0;
}
