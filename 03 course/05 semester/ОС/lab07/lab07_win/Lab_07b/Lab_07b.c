#include <windows.h>
#include <stdio.h>
#include <time.h>

int main()
{
    clock_t start = clock();

    volatile unsigned long long iters = 0;
     
    int printed5 = 0;
    int printed10 = 0;

    while (TRUE)
    {
        iters++;

        long long elapsedMs = (long long)(clock() - start) * 1000 / CLOCKS_PER_SEC;

        if (!printed5 && elapsedMs >= 5000)
        {
            printf("5  секунд:  %llu\tитераций\n", iters);
            printed5 = 1;
        }

        if (!printed10 && elapsedMs >= 10000)
        {
            printf("10 секунд:  %llu\tитераций\n", iters);
            printed10 = 1;
        }

        if (elapsedMs >= 15000)
        {
            printf("15 секунд:  %llu\tитераций\n", iters);
            break;
        }
    }

    return 0;
}
