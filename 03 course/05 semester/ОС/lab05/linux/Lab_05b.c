#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <errno.h>

void build_cpu_set_from_mask(cpu_set_t *set, unsigned long long mask) {
    CPU_ZERO(set);
    for (int cpu = 0; cpu < CPU_SETSIZE; ++cpu) {if (mask & (1ULL << cpu)) {CPU_SET(cpu, set);}}
}

int main(int argc, char *argv[])
{
    if (argc < 4) {
        printf("Использование:\n  ./Lab_05b P1 P2 P3\n\n");
        printf("P1 – целое число, маска аффинности процессоров (битовая, 0 = все)\n");
        printf("P2 – nice первого дочернего процесса\n");
        printf("P3 – nice второго дочернего процесса\n\n");
        return 1;
    }

    unsigned long long maskULL = strtoull(argv[1], NULL, 0);
    int nice1 = atoi(argv[2]);
    int nice2 = atoi(argv[3]);

    printf("Запуск с параметрами:\n");
    printf("  P1 (маска аффинности): %llu\n", maskULL);
    printf("  P2 (nice 1-го процесса): %d\n", nice1);
    printf("  P3 (nice 2-го процесса): %d\n\n", nice2);

    cpu_set_t set;
    int use_affinity = (maskULL != 0);
    if (use_affinity) { build_cpu_set_from_mask(&set, maskULL); }

    pid_t child1 = fork();
    if (child1 < 0) { perror("fork child1"); return 1; }

    if (child1 == 0) {
        if (use_affinity) {if (sched_setaffinity(0, sizeof(set), &set) == -1) {perror("child1 sched_setaffinity");} }
        if (nice(nice1) == -1) {perror("child1 nice");}
        execl("./Lab_05x", "Lab_05x", (char *)NULL);
        // execl error
        perror("child1 execl ./Lab_05x");
        _exit(1);
    }

    pid_t child2 = fork();
    if (child2 < 0) { perror("fork child2"); waitpid(child1, NULL, 0); return 1; }

    if (child2 == 0) {
        if (use_affinity) { if (sched_setaffinity(0, sizeof(set), &set) == -1) {perror("child2 sched_setaffinity");} }
        if (nice(nice2) == -1) {perror("child2 nice");}
        execl("./Lab_05x", "Lab_05x", (char *)NULL);
        // execl error
        perror("child2 execl ./Lab_05x");
        _exit(1);
    }

    printf("Дочерние процессы запущены: PID1=%d, PID2=%d\n", (int)child1, (int)child2);
    printf("Ожидание их завершения...\n");

    int status1 = 0, status2 = 0;
    waitpid(child1, &status1, 0);
    waitpid(child2, &status2, 0);

    printf("Процесс 1 завершился с кодом %d\n", WIFEXITED(status1) ? WEXITSTATUS(status1) : -1);
    printf("Процесс 2 завершился с кодом %d\n", WIFEXITED(status2) ? WEXITSTATUS(status2) : -1);

    return 0;
}
