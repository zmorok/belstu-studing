#include <stdio.h>

typedef struct {
    int id;
    int arrive;
    int burst;
    int prio;
    int left;
    int start;
    int finish;
} Proc;

int main(void)
{
    Proc proc[] = {
        {1,  0, 80, 3, 80, -1, -1},
        {2, 20, 45, 5, 45, -1, -1},
        {3, 30, 60, 2, 60, -1, -1},
        {4, 50, 30, 4, 30, -1, -1},
        {5, 70, 90, 1, 90, -1, -1},
        {6,100, 55, 3, 55, -1, -1},
        {7,120, 75, 2, 75, -1, -1},
        {8,150, 40, 5, 40, -1, -1},
        {9,180, 65, 4, 65, -1, -1},
        {10,200,85, 1, 85, -1, -1}
    };

    const int N = (int)(sizeof(proc) / sizeof(proc[0]));
    int q = 50;          // квант
    int t = 0;           // текущее "время"

    int rq[1000];               // очередь готовых задач (индексы)
    int head = 0, tail = 0;     // указатели головы/хвоста
    Proc done[20];
    int doneCount = 0;

    int queued[20] = { 0 };  // 0 – не в очереди и не была добавлена, 1 – уже добавляли

    while (doneCount < N) {
        int arrived[20];
        int arrivedCount = 0;

        // ищем задачи, которые уже пришли и ещё не ставились в очередь
        for (int i = 0; i < N; ++i) {
            if (!queued[i] && proc[i].arrive <= t) {
                arrived[arrivedCount++] = i;
            }
        }

        // сортировка по приоритету
        for (int i = 0; i < arrivedCount - 1; ++i) {
            for (int j = i + 1; j < arrivedCount; ++j) {
                int a = arrived[i];
                int b = arrived[j];
                if (proc[a].prio < proc[b].prio) {
                    int tmp = arrived[i];
                    arrived[i] = arrived[j];
                    arrived[j] = tmp;
                }
            }
        }

        // добавляем в очередь в порядке убывания приоритета
        for (int k = 0; k < arrivedCount; ++k) {
            int idx = arrived[k];
            rq[tail++] = idx;
            queued[idx] = 1;
        }

        // очередь пуста? 
        // простаиваем (увеличиваем t), 
        // чтобы по новой задачи искать
        if (head == tail) { ++t; continue; }

        // берём первый процесс из очереди
        int idx = rq[head++];
        if (proc[idx].start == -1) proc[idx].start = t;

        int slice = (proc[idx].left > q) ? q : proc[idx].left;
        proc[idx].left -= slice;
        t += slice;

        // добавляем в очередь все новые процессы, которые подошли за время этого кванта
        for (int i = 0; i < N; ++i) {
            if (!queued[i] && proc[i].arrive <= t) {
                rq[tail++] = i;
                queued[i] = 1;
            }
        }

        if (proc[idx].left > 0) {
            // процесс не завершился — отправляем его в конец очереди
            rq[tail++] = idx;
        }
        else {
            // процесс завершился
            proc[idx].finish = t;
            done[doneCount++] = proc[idx];
        }
    }

    printf("План выполнения (Round Robin, квант = %d):\n", q);

    double sumWait = 0.0, sumTurn = 0.0, sumExec = 0.0;

    for (int i = 0; i < doneCount; ++i) {
        Proc p = done[i];
        int wait = p.start - p.arrive;
        int turn = p.finish - p.arrive;
        int exec = p.finish - p.start;

        printf("Процесс %2d | старт: %4d | конец: %4d | приоритет: %d\n",
            p.id, p.start, p.finish, p.prio);

        sumWait += wait;
        sumTurn += turn;
        sumExec += exec;
    }

    int n = doneCount;
    printf("\nСреднее время ожидания:\t\t%.2f\n", sumWait / n);
    printf("Среднее время выполнения:\t%.2f\n", sumExec / n);
    printf("Среднее время обращения:\t%.2f\n", sumTurn / n);
    printf("Пропускная способность:\t\t%.4f задач/ед. времени\n", (float)n / done[n - 1].finish);

    return 0;
}
