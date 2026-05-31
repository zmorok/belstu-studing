#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/timerfd.h>
#include <time.h>
#include <fcntl.h>

int main(void)
{
    int tfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if (tfd < 0)
    {
        perror("timerfd_create");
        return 1;
    }

    struct itimerspec ts;
    ts.it_value.tv_sec = 3;
    ts.it_value.tv_nsec = 0;
    ts.it_interval.tv_sec = 3;
    ts.it_interval.tv_nsec = 0;

    if (timerfd_settime(tfd, 0, &ts, NULL) < 0)
    {
        perror("timerfd_settime");
        close(tfd);
        return 1;
    }

    unsigned long long iterationCount = 0;
    int periodsElapsed = 0;

    struct timespec start, now;
    clock_gettime(CLOCK_MONOTONIC, &start);

    while (1)
    {
        iterationCount++;

        uint64_t expirations;
        ssize_t r = read(tfd, &expirations, sizeof(expirations));
        if (r == sizeof(expirations))
        {
            periodsElapsed += expirations;
            printf("[После %02d секунд] Итерации: %llu\n", periodsElapsed * 3, iterationCount);
        }

        clock_gettime(CLOCK_MONOTONIC, &now);
        long long elapsed_ms =
            (now.tv_sec - start.tv_sec) * 1000LL +
            (now.tv_nsec - start.tv_nsec) / 1000000LL;

        if (elapsed_ms >= 15000)
        {
            printf("[После 15 секунд] Итерации: %llu\n", iterationCount);
            break;
        }
    }

    close(tfd);
    return 0;
}
