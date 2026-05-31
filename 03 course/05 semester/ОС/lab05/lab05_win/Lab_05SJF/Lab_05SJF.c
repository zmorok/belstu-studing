#include <stdio.h>

typedef struct {
    int id;
    int arrive;
    int burst;
    int prio;
    int start;
    int finish;
} Job;

int main() {
    Job queue[] = {
        {1, 0,  80, 3, 0,0},
        {2, 20, 45, 5, 0,0},
        {3, 30, 60, 2, 0,0},
        {4, 50, 30, 4, 0,0},
        {5, 70, 90, 1, 0,0},
        {6, 100,55, 3, 0,0},
        {7, 120,75, 2, 0,0},
        {8, 150,40, 5, 0,0},
        {9, 180,65, 4, 0,0},
        {10,200,85, 1, 0,0}
    };

    int total = sizeof(queue) / sizeof(Job);
    Job result[20];     // задачи будут в порядке выполнения, итоговые
    int resCount = 0;

    int t = 0;          // текущее время

    int done[20] = { 0 };

    int completed = 0;

    while (completed < total) {
        int readyIdx[20];   // очередь готовых задач на момент t
        int readyCount = 0;

        for (int i = 0; i < total; i++)
            if (!done[i] && queue[i].arrive <= t)
                readyIdx[readyCount++] = i;
            
        if (readyCount == 0) { t++; continue; }
                            // очередь пуста? 
                            // простаиваем (увеличиваем t), 
                            // чтобы по новой задачи искать

        // поиск лучшей по времени burst здачи (sjf)
        int best = readyIdx[0];
        for (int k = 1; k < readyCount; k++) {
            int idx = readyIdx[k];
            if (queue[idx].burst < queue[best].burst) best = idx;
        }

        Job cur = queue[best];
        cur.start = t;
        cur.finish = t + cur.burst;

        t = cur.finish;

        result[resCount++] = cur;
        done[best] = 1;
        completed++;
    }

    printf("План выполнения алгоритма shortest job first:\n");
    double sumWait = 0, sumTurn = 0, sumExec = 0;

    for (int i = 0; i < resCount; i++) {
        Job j = result[i];

        int wait = j.start - j.arrive;
        int turn = j.finish - j.arrive;

        printf("Задача %d\t| Старт: %d\t| Конец: %d\t| Приоритет: %d\n", j.id, j.start, j.finish, j.prio);

        sumWait += wait;
        sumTurn += turn;
        sumExec += j.burst;
    }

    printf("\nСреднее время ожидания:\t%.2f\n", sumWait / resCount);
    printf("Среднее выполнение:\t%.2f\n", sumExec / resCount);
    printf("Среднее обращение:\t%.2f\n", sumTurn / resCount);
    printf("Пропускная способность:\t%.4f\n", (float)resCount / result[resCount - 1].finish);

    return 0;
}