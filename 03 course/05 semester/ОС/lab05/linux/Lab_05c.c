#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sched.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/resource.h>

typedef struct ThreadParams {
    int id;                  // номер потока (1 или 2 для вывода)
    int nice_value;          // желаемый nice для этого потока
    int use_affinity;        // 0 - не трогать аффинность, 1 - установить
    cpu_set_t cpu_set;       // маска аффинности для потока
} ThreadParams;

void build_cpu_set_from_mask(cpu_set_t *set, unsigned long long mask) {
    CPU_ZERO(set);
    for (int cpu = 0; cpu < CPU_SETSIZE; ++cpu) {if (mask & (1ULL << cpu)) {CPU_SET(cpu, set);}}
}

void* WorkerThread(void *arg)
{
    ThreadParams *params = (ThreadParams*)arg;
    int id = params->id;

    if (params->use_affinity) { if (sched_setaffinity(0, sizeof(params->cpu_set), &params->cpu_set) == -1) {perror("sched_setaffinity (thread)");}}
    
    pid_t tid = gettid();
    if (setpriority(PRIO_PROCESS, tid, params->nice_value) == -1) {perror("setpriority (thread)");}

    pid_t pid = getpid();

    struct timespec ts, t1, t2;
    ts.tv_sec  = 0;
    ts.tv_nsec = 200 * 1000000L; 

    clock_gettime(CLOCK_MONOTONIC, &t1);

    for (int i = 1; i <= 1000000; ++i) {
        if (i % 1000 == 0) {
            int curNice = getpriority(PRIO_PROCESS, tid);
            int cpu     = sched_getcpu();

            printf("[Поток %d] Итерация: %d\tPID=%d\tTID=%d\tnice=%d\tCPU=%d\n",
                   id, i, (int)pid, (int)tid, curNice, cpu);

            nanosleep(&ts, NULL);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &t2);

    double elapsed = (t2.tv_sec - t1.tv_sec) + (t2.tv_nsec - t1.tv_nsec) / 1e9;
    printf("! Поток %d завершился за %.3f секунд\n", id, elapsed);

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc < 4) {
        printf("Использование:\n  ./Lab_05c P1 P2 P3\n\n");
        printf("P1 – целое число, маска родственности процессоров\n");
        printf("P2 – целое число, задающее nice первого дочернего потока\n");
        printf("P3 – целое число, задающее nice второго дочернего потока\n\n");
        return 1;
    }

    unsigned long long maskULL = strtoull(argv[1], NULL, 0);
    int nice1 = atoi(argv[2]);
    int nice2 = atoi(argv[3]);

    printf("Запуск с параметрами:\n");
    printf("  P1 (маска аффинности): %llu\n", maskULL);
    printf("  P2 (nice 1-го потока): %d\n", nice1);
    printf("  P3 (nice 2-го потока): %d\n\n", nice1, nice2);

    int use_affinity = (maskULL != 0);

    ThreadParams tp1, tp2;
    tp1.id = 1;
    tp1.nice_value = nice1;
    tp1.use_affinity = use_affinity;

    tp2.id = 2;
    tp2.nice_value = nice2;
    tp2.use_affinity = use_affinity;

    if (use_affinity) {
        build_cpu_set_from_mask(&tp1.cpu_set, maskULL);
        build_cpu_set_from_mask(&tp2.cpu_set, maskULL);
    }

    pthread_t th1, th2;

    if (pthread_create(&th1, NULL, WorkerThread, &tp1) != 0) {perror("pthread_create th1");return 1;}

    if (pthread_create(&th2, NULL, WorkerThread, &tp2) != 0) {perror("pthread_create th2");pthread_join(th1, NULL);return 1;}

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    printf("Оба потока завершили работу.\n");
    return 0;
}
