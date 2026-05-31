#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STATS_INTERVAL_MS 5000

// синхронизация
pthread_mutex_t cs;
pthread_cond_t  canProduce;
pthread_cond_t  canConsume;

// буфер
int* buffer = NULL;
int bufferMaxSize = 0, bufferCount = 0;
int head = 0, tail = 0;
int nextItemId = 1;

// задержки (мс)
int prodDelayMin = 0, prodDelayMax = 0;
int consDelayMin = 0, consDelayMax = 0;

// производитель
unsigned long long totalProduced = 0;
unsigned long long totalProducerWorkTimeMs = 0;
unsigned long long lastProduceTimeMs = 0;   // время последнего производства в мс
unsigned long long sumProduceIntervalsMs = 0;
unsigned long long produceIntervalsCount = 0;

// потребитель
unsigned long long totalConsumed = 0;
unsigned long long totalConsumerWorkTimeMs = 0;

// для статистики по периодам
unsigned long long lastStatsProduced = 0;
unsigned long long lastStatsConsumed = 0;

// стартовое время приложения
struct timespec appStartTime;

// --- утилиты времени ---

static unsigned long long timespec_to_ms(const struct timespec* t)
{
    return (unsigned long long)t->tv_sec * 1000ULL +
           (unsigned long long)t->tv_nsec / 1000000ULL;
}

static double timespec_diff_sec(const struct timespec* t2,
                                const struct timespec* t1)
{
    time_t ds = t2->tv_sec - t1->tv_sec;
    long   dn = t2->tv_nsec - t1->tv_nsec;
    return (double)ds + (double)dn / 1e9;
}

static void sleep_ms(int delay_ms)
{
    if (delay_ms <= 0) return;
    struct timespec ts;
    ts.tv_sec  = delay_ms / 1000;
    ts.tv_nsec = (delay_ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

static int random_in_range(int min, int max)
{
    if (max <= min) return min;
    int diff = max - min + 1;
    return min + rand() % diff;
}

// поток производителя
static void* ProducerThread(void* arg)
{
    (void)arg;
    for (;;)
    {
        int delay = random_in_range(prodDelayMin, prodDelayMax);
        sleep_ms(delay);

        struct timespec t_now;
        clock_gettime(CLOCK_MONOTONIC, &t_now);
        unsigned long long now_ms = timespec_to_ms(&t_now);

        pthread_mutex_lock(&cs);

        while (bufferCount == bufferMaxSize) {
            printf("!  full   [Producer] Буфер полон, блокировка производителя\n");
            fflush(stdout);
            pthread_cond_wait(&canProduce, &cs);
            printf("          [Producer] Производитель пробуждён\n\n");
            fflush(stdout);
        }

        // добавили в буфер
        int item = nextItemId++;
        buffer[tail] = item;
        tail = (tail + 1) % bufferMaxSize;
        bufferCount++;

        // метрики производителя
        totalProduced++;
        totalProducerWorkTimeMs += delay;

        if (totalProduced > 1) {
            unsigned long long interval = now_ms - lastProduceTimeMs;
            sumProduceIntervalsMs += interval;
            produceIntervalsCount++;
        }
        lastProduceTimeMs = now_ms;

        printf("++++++++++[Producer] Добавлен элемент %d, в буфере сейчас %d (пробуждаем потоки потребителя)\n",
               item, bufferCount);
        fflush(stdout);

        pthread_cond_signal(&canConsume);

        pthread_mutex_unlock(&cs);
    }
    return NULL;
}

// поток потребителя
static void* ConsumerThread(void* arg)
{
    (void)arg;
    for (;;)
    {
        int delay = random_in_range(consDelayMin, consDelayMax);
        sleep_ms(delay);

        pthread_mutex_lock(&cs);

        while (bufferCount == 0) {
            printf("?  empty  [Consumer] Буфер пуст, блокировка потребителя\n");
            fflush(stdout);
            pthread_cond_wait(&canConsume, &cs);
            printf("          [Consumer] Потребитель пробуждён\n\n");
            fflush(stdout);
        }

        int item = buffer[head];
        head = (head + 1) % bufferMaxSize;
        bufferCount--;

        totalConsumed++;
        totalConsumerWorkTimeMs += delay;

        printf("----------[Consumer] Извлечён элемент %d, в буфере сейчас %d (пробуждаем потоки производителя)\n",
               item, bufferCount);
        fflush(stdout);

        pthread_cond_signal(&canProduce);

        pthread_mutex_unlock(&cs);
    }
    return NULL;
}

// поток статистики
static void* StatsThread(void* arg)
{
    (void)arg;

    for (;;)
    {
        struct timespec ts;
        ts.tv_sec  = STATS_INTERVAL_MS / 1000;
        ts.tv_nsec = (STATS_INTERVAL_MS % 1000) * 1000000L;
        nanosleep(&ts, NULL);

        pthread_mutex_lock(&cs);

        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        double appSeconds = timespec_diff_sec(&now, &appStartTime);

        unsigned long long producedTotal = totalProduced;
        unsigned long long consumedTotal = totalConsumed;
        int currentCount = bufferCount;

        unsigned long long producedDelta = producedTotal - lastStatsProduced;
        unsigned long long consumedDelta = consumedTotal - lastStatsConsumed;

        lastStatsProduced = producedTotal;
        lastStatsConsumed = consumedTotal;

        double bufferPercent = (bufferMaxSize > 0)
            ? (100.0 * currentCount / bufferMaxSize)
            : 0.0;

        double prodSpeed = producedDelta / (STATS_INTERVAL_MS / 1000.0);
        double consSpeed = consumedDelta / (STATS_INTERVAL_MS / 1000.0);

        double avgProduceIntervalMs = 0.0;
        if (produceIntervalsCount > 0)
            avgProduceIntervalMs =
                (double)sumProduceIntervalsMs / produceIntervalsCount;

        double avgConsumeTimeMs = 0.0;
        if (consumedTotal > 0)
            avgConsumeTimeMs =
                (double)totalConsumerWorkTimeMs / consumedTotal;

        double totalProducerTimeSec = totalProducerWorkTimeMs / 1000.0;
        double totalConsumerTimeSec = totalConsumerWorkTimeMs / 1000.0;

        printf("\n########## Статистика (t = %.1f c) ##########\n", appSeconds);
        printf("Общее количество произведённых элементов: %llu\n", producedTotal);
        printf("Общее количество потреблённых элементов: %llu\n", consumedTotal);
        printf("Текущее количество элементов в буфере: %d\n", currentCount);
        printf("Процент заполнения буфера: %.1f%%\n", bufferPercent);
        printf("Произведено за последний период: %llu\n", producedDelta);
        printf("Среднее время между производством элементов: %.2f мс\n", avgProduceIntervalMs);
        printf("Текущая скорость производства: %.2f эл./сек\n", prodSpeed);
        printf("Общее время работы производителя: %.2f сек\n", totalProducerTimeSec);
        printf("Потреблено за последний период: %llu\n", consumedDelta);
        printf("Среднее время обработки элемента (потребитель): %.2f мс\n", avgConsumeTimeMs);
        printf("Текущая скорость потребления: %.2f эл./сек\n", consSpeed);
        printf("Общее время работы потребителя: %.2f сек\n", totalConsumerTimeSec);
        printf("=============================================\n\n");
        fflush(stdout);

        pthread_mutex_unlock(&cs);
    }

    return NULL;
}

int main(int argc, char* argv[])
{
    if (argc != 6) {
        fprintf(stderr,
            "Использование:\n"
            "%s [buffer_size] [prod_delay_min] [prod_delay_max] [cons_delay_min] [cons_delay_max]\n"
            "Пример: %s 10 0 100 50 150\n",
            argv[0], argv[0]);
        return 1;
    }

    bufferMaxSize = atoi(argv[1]);
    prodDelayMin  = atoi(argv[2]);
    prodDelayMax  = atoi(argv[3]);
    consDelayMin  = atoi(argv[4]);
    consDelayMax  = atoi(argv[5]);

    if (bufferMaxSize <= 0) {
        fprintf(stderr, "Некорректный размер буфера\n");
        return 2;
    }

    if (prodDelayMin < 0 || prodDelayMax < prodDelayMin ||
        consDelayMin < 0 || consDelayMax < consDelayMin) {
        fprintf(stderr, "Некорректные диапазоны задержек\n");
        return 3;
    }

    const char* envUser = getenv("USER");
    if (!envUser || envUser[0] == '\0') {
        fprintf(stderr, "Не удалось получить имя пользователя из переменной USER\n");
        return 4;
    }

    buffer = (int*)malloc(bufferMaxSize * sizeof(int));
    if (!buffer) {
        fprintf(stderr, "Не удалось выделить память под буфер\n");
        return 5;
    }

    if (pthread_mutex_init(&cs, NULL) != 0) {
        perror("pthread_mutex_init");
        free(buffer);
        return 6;
    }

    if (pthread_cond_init(&canProduce, NULL) != 0) {
        perror("pthread_cond_init(canProduce)");
        pthread_mutex_destroy(&cs);
        free(buffer);
        return 7;
    }

    if (pthread_cond_init(&canConsume, NULL) != 0) {
        perror("pthread_cond_init(canConsume)");
        pthread_cond_destroy(&canProduce);
        pthread_mutex_destroy(&cs);
        free(buffer);
        return 8;
    }

    srand((unsigned int)time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &appStartTime);

    printf("Имя пользователя: %s\n", envUser);
    printf("Текущий размер буфера: %d\n", bufferCount);
    printf("Максимальный размер буфера: %d\n\n", bufferMaxSize);
    fflush(stdout);

    pthread_t prodThread, consThread, statThread;

    if (pthread_create(&prodThread, NULL, ProducerThread, NULL) != 0) {
        perror("pthread_create(Producer)");
        pthread_cond_destroy(&canConsume);
        pthread_cond_destroy(&canProduce);
        pthread_mutex_destroy(&cs);
        free(buffer);
        return 9;
    }

    if (pthread_create(&consThread, NULL, ConsumerThread, NULL) != 0) {
        perror("pthread_create(Consumer)");
        pthread_cancel(prodThread);
        pthread_join(prodThread, NULL);
        pthread_cond_destroy(&canConsume);
        pthread_cond_destroy(&canProduce);
        pthread_mutex_destroy(&cs);
        free(buffer);
        return 10;
    }

    if (pthread_create(&statThread, NULL, StatsThread, NULL) != 0) {
        perror("pthread_create(Stats)");
        pthread_cancel(prodThread);
        pthread_cancel(consThread);
        pthread_join(prodThread, NULL);
        pthread_join(consThread, NULL);
        pthread_cond_destroy(&canConsume);
        pthread_cond_destroy(&canProduce);
        pthread_mutex_destroy(&cs);
        free(buffer);
        return 11;
    }

    pthread_join(prodThread, NULL);
    pthread_join(consThread, NULL);
    pthread_join(statThread, NULL);

    pthread_cond_destroy(&canConsume);
    pthread_cond_destroy(&canProduce);
    pthread_mutex_destroy(&cs);
    free(buffer);

    return 0;
}