#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/resource.h>
#include <sched.h>

int main()
{
    struct timespec ts, t1, t2;
    ts.tv_sec = 0;
    ts.tv_nsec = 200 * 1000000L;

    pid_t pid = getpid();
    pid_t tid = gettid();

    clock_gettime(CLOCK_MONOTONIC, &t1);

    for (int i = 1; i <= 1000000; ++i)
    {
        if (i % 1000 == 0)
        {
            int niceVal = getpriority(PRIO_PROCESS, 0);
            int cpu     = sched_getcpu();
            printf("Итерация: %d\t", i);
            printf("PID: %d\t", pid);
            printf("TID: %d\t", tid);
            printf("nice: %d\t", niceVal);
            printf("CPU: %d\n", cpu);
            nanosleep(&ts, NULL);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &t2);

    double elapsed = (t2.tv_sec - t1.tv_sec) + (t2.tv_nsec - t1.tv_nsec) / 1e9;

    printf("\nВремя, прошедшее с момента запуска (clock): %.3f секунд(ы)\n", elapsed);

    return 0;
}
