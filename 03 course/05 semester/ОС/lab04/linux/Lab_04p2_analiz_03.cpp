#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>         // sysconf, fork, execvp
#include <sys/types.h>
#include <sys/wait.h>       // waitpid
#include <time.h>           // clock_gettime

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++) if (n % i == 0) return false;
    return true;
}

int WorkerMain(int argc, char* argv[])
{
    if (argc != 4) {
        fprintf(stderr, "worker: неверное число аргументов\n");
        return 1;
    }

    char* endptr = NULL;
    long startL = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0') return 1;
    long endL   = strtol(argv[3], &endptr, 10);
    if (*endptr != '\0') return 1;

    int start = (int)startL;
    int end   = (int)endL;

    int count = 0;
    for (int n = start; n <= end; ++n) if (isPrime(n)) ++count;

    _exit((unsigned char)(count & 0xFF));
}

double diff_ms(struct timespec a, struct timespec b)
{
    long sec  = b.tv_sec  - a.tv_sec;
    long nsec = b.tv_nsec - a.tv_nsec;
    return (double)sec * 1000.0 + (double)nsec / 1e6;
}

int main(int argc, char* argv[])
{

    if (argc >= 2 && strcmp(argv[1], "worker") == 0) return WorkerMain(argc, argv);
    

    if (argc != 3) {
        printf("Использование для измерений:\n");
        printf("  %s <Нижний_порог> <Верхний_порог>\n", argv[0]);
        printf("Например: %s 1 2097152\n", argv[0]);
        return 1;
    }

    char* endptr = NULL;
    long lowL  = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0') { printf("Ошибка: некорректный нижний порог диапазона\n");  return 1; }

    long highL = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0') { printf("Ошибка: некорректный верхний порог диапазона\n"); return 1; }

    if (lowL > highL) { printf("Ошибка: нижний порог больше верхнего\n"); return 1; }

    int low  = (int)lowL;
    int high = (int)highL;

    // число логических ядер
    long cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (cores < 1) cores = 1;
    int maxProcs = (int)(2 * cores);

    printf("Диапазон: [%d; %d], логических процессоров: %ld, процессы: 1..%d (x2)\n", low, high, cores, maxProcs);
    printf("Procs\tTime_ms\tPrimesCount_(mod_256)\n");

    const char* self = argv[0];
    for (int procCount = 1; procCount <= maxProcs; procCount *= 2) {
        int totalRange = high - low + 1;
        int baseSize   = totalRange / procCount;
        int remainder  = totalRange % procCount;

        int current = low;
        int totalPrimes = 0;

        struct timespec t1, t2;
        clock_gettime(CLOCK_MONOTONIC, &t1);

        // дочерние процессы 
        for (int i = 0; i < procCount; ++i) {
            int partSize = baseSize + (i < remainder ? 1 : 0);
            int start    = current;
            int end      = start + partSize - 1;

            if (partSize <= 0) { start = 0; end = -1; }

            current = end + 1;

            pid_t pid = fork();
            if (pid < 0) {
                perror("fork");
                procCount = maxProcs + 1;   // выход из внешнего цикла
                break;
            }

            if (pid == 0) {
                // дочерний процесс: exec того же бинарника в режиме worker 
                char startStr[32], endStr[32];
                snprintf(startStr, sizeof(startStr), "%d", start);
                snprintf(endStr, sizeof(endStr), "%d", end);

                char* args[] = { (char*)self, (char *)("worker"), startStr, endStr, NULL };
                execvp(args[0], args);

                perror("execvp"); // здесь уже ошибка
                _exit(1);
            }
        }

        if (procCount > maxProcs) break;

        // ожидание всех дочерних процессов и суммирование их кодоов возвратов
        for (int i = 0; i < procCount; ++i) {
            int status = 0;
            pid_t w = wait(&status);
            if (w == -1) { perror("wait"); continue; }
            if (WIFEXITED(status)) { int code = WEXITSTATUS(status); totalPrimes += code; }
        }

        clock_gettime(CLOCK_MONOTONIC, &t2);
        double elapsed_ms = diff_ms(t1, t2);

        printf("%d\t%.3f\t%d\n", procCount, elapsed_ms, totalPrimes);
    }

    return 0;
}
