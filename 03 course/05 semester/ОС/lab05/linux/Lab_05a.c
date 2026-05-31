#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sched.h>
#include <locale.h>

const char* PolicyName(int policy)
{
    switch (policy)
    {
        case SCHED_OTHER:   return "SCHED_OTHER";
        case SCHED_FIFO:    return "SCHED_FIFO";
        case SCHED_RR:      return "SCHED_RR";
        case SCHED_BATCH:   return "SCHED_BATCH";
        case SCHED_IDLE:    return "SCHED_IDLE";
        case SCHED_DEADLINE:return "SCHED_DEADLINE";
        default:            return "UNKNOWN";
    }
}

int main()
{
    pid_t pid = getpid();
    pid_t tid = gettid();

    int niceVal = getpriority(PRIO_PROCESS, 0);
    int policy  = sched_getscheduler(0);

    struct sched_param param;
    sched_getparam(0, &param);
    int threadPriority = param.sched_priority;

    cpu_set_t affinity;
    CPU_ZERO(&affinity);

    if (sched_getaffinity(0, sizeof(affinity), &affinity) == -1)
    {
        perror("sched_getaffinity");
        return 1;
    }

    long nproc = sysconf(_SC_NPROCESSORS_CONF);
    if (nproc <= 0) nproc = CPU_SETSIZE;

    char procMaskBits[CPU_SETSIZE + 1];
    int pos = 0;
    int cpuCount = 0;

    for (long i = nproc - 1; i >= 0; --i)
    {
        if (CPU_ISSET(i, &affinity)) { procMaskBits[pos++] = '1'; cpuCount++; }
        else { procMaskBits[pos++] = '0'; }
    }
    procMaskBits[pos] = '\0';

    int currentCpu = sched_getcpu();

    printf("Идентификатор текущего процесса: %d\n", pid);
    printf("Идентификатор текущего потока: %d\n", tid);
    printf("Планировщик текущего процесса: %d (%s)\n", policy, PolicyName(policy));
    printf("Приоритет процесса: %d\n", niceVal);
    printf("Приоритет текущего потока: %d\n", threadPriority);
    printf("Маска родственности процесса в двоичном виде: %s\n", procMaskBits);
    printf("Количество процессоров, доступных процессу: %d\n", cpuCount);
    printf("Номер процессора, на котором выполняется поток: %d\n", currentCpu);

    return 0;
}
