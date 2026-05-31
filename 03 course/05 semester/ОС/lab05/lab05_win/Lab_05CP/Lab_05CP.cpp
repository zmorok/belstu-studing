#include <stdio.h>

typedef struct {
    int id;
    int arrive;
    int burst;
    int prio;
    int rest;
    int start;
    int finish;
} Job;

int main()
{
    Job jobs[] = {
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

    const int N = (int)(sizeof(jobs) / sizeof(jobs[0]));
    Job done[20];
    int doneCount = 0;

    int now = 0;

    while (doneCount < N) {
        int bestIdx = -1;
        int bestPrio = -1;

        // ищем лучшую по приоритету задачу среди уже поступивших и незавершённых
        for (int i = 0; i < N; ++i) {
            if (jobs[i].arrive <= now && jobs[i].rest > 0) {
                if (bestIdx == -1 || jobs[i].prio > bestPrio) {
                    bestIdx = i;
                    bestPrio = jobs[i].prio;
                }
            }
        }

        // если готовых задач нет — просто двигаем время вперёд
        if (bestIdx == -1) {
            ++now;
            continue;
        }

        Job* j = &jobs[bestIdx];

        if (j->start == -1) j->start = now;

        j->rest--; ++now;

        if (j->rest == 0) {
            j->finish = now;
            done[doneCount++] = *j;
        }
    }

    printf("План выполнения (приоритетный вытесняющий алгоритм):\n");

    double sumWait = 0.0;
    double sumTurn = 0.0;
    double sumExec = 0.0;

    for (int i = 0; i < doneCount; ++i) {
        Job j = done[i];
        int wait = j.start - j.arrive;
        int turn = j.finish - j.arrive;
        int exec = j.finish - j.start;

        printf("Процесс %2d | старт: %4d | конец: %4d | приоритет: %d\n", j.id, j.start, j.finish, j.prio);

        sumWait += wait;
        sumTurn += turn;
        sumExec += exec;
    }

    int n = doneCount;
    printf("\nСреднее время ожидания:  \t%.2f\n", sumWait / n);
    printf("Среднее время выполнения:\t%.2f\n", sumExec / n);
    printf("Среднее время обращения:\t%.2f\n", sumTurn / n);
    printf("Пропускная способность:  \t%.4f задач/ед. времени\n", (float)n / done[n - 1].finish);

    return 0;
}