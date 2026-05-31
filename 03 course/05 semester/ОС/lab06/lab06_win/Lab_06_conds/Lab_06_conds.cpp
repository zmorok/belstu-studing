#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STATS_INTERVAL_MS 5000

clock_t start = clock();

// синхронизация
CRITICAL_SECTION cs;
CONDITION_VARIABLE canProduce, canConsume;

// буфер
int* buffer = NULL;
int bufferMaxSize = 0, bufferCount = 0;
int head = 0, tail = 0;
int nextItemId = 1;

// задержки
int prodDelayMin = 0, prodDelayMax = 0;
int consDelayMin = 0, consDelayMax = 0;


// производитель
unsigned long long totalProduced = 0;
unsigned long long totalProducerWorkTimeMs = 0;
unsigned long long lastProduceTime = 0;
unsigned long long sumProduceIntervalsMs = 0;
unsigned long long produceIntervalsCount = 0;

// потребитель
unsigned long long totalConsumed = 0;
unsigned long long totalConsumerWorkTimeMs = 0;

// для статистики по периодам
unsigned long long lastStatsProduced = 0;
unsigned long long lastStatsConsumed = 0;


static int random_in_range(int min, int max)
{
    if (max <= min) return min;
    int diff = max - min + 1;
    return min + rand() % diff;
}

// поток производителя
static DWORD WINAPI ProducerThread(LPVOID lpParam)
{
    (void)lpParam;
    for (;;)
    {
        int delay = random_in_range(prodDelayMin, prodDelayMax);
        Sleep(delay);

        clock_t now = clock();

        EnterCriticalSection(&cs);

        while (bufferCount == bufferMaxSize) {
            printf("!  full   [Producer] Буфер полон, блокировка производителя\n");
            fflush(stdout);
            SleepConditionVariableCS(&canProduce, &cs, INFINITE);
            printf("          [Producer] Производитель пробуждён\n\n");
            fflush(stdout);
        }

        // добавили в буфер
        int item = nextItemId++;
        buffer[tail] = item;
        tail = (tail + 1) % bufferMaxSize;
        bufferCount++;

        // метрика производителей
        totalProduced++;
        totalProducerWorkTimeMs += delay;

        if (totalProduced > 1) {
            ULONGLONG interval = now - lastProduceTime;
            sumProduceIntervalsMs += interval;
            produceIntervalsCount++;
        }
        lastProduceTime = now;

        printf("++++++++++[Producer] Добавлен элемент %d, в буфере сейчас %d (пробуждаем потоки потребителя)\n", item, bufferCount);
        fflush(stdout);

        WakeConditionVariable(&canConsume);

        LeaveCriticalSection(&cs);
    }
    return 0;
}

// поток потребителя
static DWORD WINAPI ConsumerThread(LPVOID lpParam)
{
    (void)lpParam;
    for (;;)
    {
        int delay = random_in_range(consDelayMin, consDelayMax);
        Sleep(delay);

        EnterCriticalSection(&cs);

        while (bufferCount == 0) {
            printf("?  empty  [Consumer] Буфер пуст, блокировка потребителя\n");
            fflush(stdout);
            SleepConditionVariableCS(&canConsume, &cs, INFINITE);
            printf("          [Consumer] Потребитель пробуждён\n\n");
            fflush(stdout);
        }

        int item = buffer[head];
        head = (head + 1) % bufferMaxSize;
        bufferCount--;

        totalConsumed++;
        totalConsumerWorkTimeMs += delay;

        printf("----------[Consumer] Извлечён элемент %d, в буфере сейчас %d (пробуждаем потоки производителя)\n", item, bufferCount);
        fflush(stdout);

        WakeConditionVariable(&canProduce);

        LeaveCriticalSection(&cs);
    }
    return 0;
}

// поток статистики
DWORD WINAPI StatsThread(LPVOID lpParam)
{
    (void)lpParam;

    for (;;)
    {
        Sleep(STATS_INTERVAL_MS);

        EnterCriticalSection(&cs);

        clock_t now = clock();
        double appSeconds = now - start;

        unsigned long long producedTotal = totalProduced;
        unsigned long long consumedTotal = totalConsumed;
        int currentCount = bufferCount;

        unsigned long long producedDelta = producedTotal - lastStatsProduced;
        unsigned long long consumedDelta = consumedTotal - lastStatsConsumed;

        lastStatsProduced = producedTotal;
        lastStatsConsumed = consumedTotal;

        double bufferPercent = (bufferMaxSize > 0) ? (100.0 * currentCount / bufferMaxSize) : 0.0;

        double prodSpeed = producedDelta / (STATS_INTERVAL_MS / 1000.0);
        double consSpeed = consumedDelta / (STATS_INTERVAL_MS / 1000.0);

        double avgProduceIntervalMs = 0.0;
        if (produceIntervalsCount > 0)  avgProduceIntervalMs = (double)sumProduceIntervalsMs / produceIntervalsCount;
        
        double avgConsumeTimeMs = 0.0;
        if (consumedTotal > 0) avgConsumeTimeMs = (double)totalConsumerWorkTimeMs / consumedTotal;
        

        double totalProducerTimeSec = totalProducerWorkTimeMs / 1000.0;
        double totalConsumerTimeSec = totalConsumerWorkTimeMs / 1000.0;

        printf("\n########## Статистика (t = %.1f c) ##########\n", appSeconds / 1000);
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

        LeaveCriticalSection(&cs);
    }

    return 0;
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
    prodDelayMin = atoi(argv[2]);
    prodDelayMax = atoi(argv[3]);
    consDelayMin = atoi(argv[4]);
    consDelayMax = atoi(argv[5]);

    if (bufferMaxSize <= 0) { fprintf(stderr, "Некорректный размер буфера\n"); return 2; }

    if (prodDelayMin < 0 || prodDelayMax < prodDelayMin || consDelayMin < 0 || consDelayMax < consDelayMin) { fprintf(stderr, "Некорректные диапазоны задержек\n"); return 3; }

    char username[256];
    DWORD usernameLen = sizeof(username);
    if (!GetUserNameA(username, &usernameLen)) { fprintf(stderr, "GetUserNameA failed: %lu\n", GetLastError()); return 4; }

    buffer = (int*)malloc(bufferMaxSize * sizeof(int));
    if (!buffer) { fprintf(stderr, "Не удалось выделить память под буфер\n"); return 5; }

    InitializeCriticalSection(&cs);
    InitializeConditionVariable(&canProduce);
    InitializeConditionVariable(&canConsume);

    srand(time(NULL));

    printf("Имя пользователя: %s\n", username);
    printf("Текущий размер буфера: %d\n", bufferCount);
    printf("Максимальный размер буфера: %d\n\n", bufferMaxSize);
    fflush(stdout);

    HANDLE hProd = CreateThread(NULL, 0, ProducerThread, NULL, 0, NULL);
    HANDLE hCons = CreateThread(NULL, 0, ConsumerThread, NULL, 0, NULL);
    HANDLE hStat = CreateThread(NULL, 0, StatsThread, NULL, 0, NULL);

    if (!hProd || !hCons || !hStat) { fprintf(stderr, "CreateThread failed: %lu\n", GetLastError()); return 6; }

    WaitForSingleObject(hProd, INFINITE);
    WaitForSingleObject(hCons, INFINITE);
    WaitForSingleObject(hStat, INFINITE);

    CloseHandle(hProd);
    CloseHandle(hCons);
    CloseHandle(hStat);

    DeleteCriticalSection(&cs);
    free(buffer);

    return 0;
}