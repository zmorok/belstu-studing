#include <stdio.h>
#include <time.h>
#include <stdint.h>

static long long ns_diff(const struct timespec* a, const struct timespec* b)
{
    return (long long)(b->tv_sec - a->tv_sec) * 1000000000LL + (long long)(b->tv_nsec - a->tv_nsec);
}

int main(void)
{
    struct timespec cpu_start, cpu_now;
    struct timespec real_start, real_end;

    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cpu_start) != 0)
    { perror("clock_gettime(CLOCK_PROCESS_CPUTIME_ID)"); return 1; }

    if (clock_gettime(CLOCK_MONOTONIC, &real_start) != 0)
    { perror("clock_gettime(CLOCK_MONOTONIC)"); return 1; }

    volatile unsigned long long iters = 0;

    int printed5 = 0;
    int printed10 = 0;

    for (;;)
    {
        iters++;

        if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cpu_now) != 0)
        { perror("clock_gettime(CLOCK_PROCESS_CPUTIME_ID)"); return 1; }

        long long cpu_ms = ns_diff(&cpu_start, &cpu_now) / 1000000LL;

        if (!printed5 && cpu_ms >= 5000)
        {
            printf("5  секунд CPU: %llu\tитераций\n", iters);
            printed5 = 1;
        }

        if (!printed10 && cpu_ms >= 10000)
        {
            printf("10 секунд CPU: %llu\tитераций\n", iters);
            printed10 = 1;
        }

        if (cpu_ms >= 15000)
        {
            printf("15 секунд CPU: %llu\tитераций\n", iters);
            break;
        }
    }

    if (clock_gettime(CLOCK_MONOTONIC, &real_end) != 0)
    {
        perror("clock_gettime(CLOCK_MONOTONIC)");
        return 1;
    }

    long long real_ns = ns_diff(&real_start, &real_end);
    double real_sec = (double)real_ns / 1e9;

    printf("Реальное время: %.6f секунд\n", real_sec);
    return 0;
}