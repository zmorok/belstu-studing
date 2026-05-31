#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>      // O_CREAT, O_EXCL
#include <sys/stat.h>   // mode constants

#define ITERATIONS 90
#define SEM_NAME "/lab_06b_sem"

char username[256];
int  nameLen = 0;

sem_t* sem = NULL;

static void work(const char* procName)
{
    pid_t pid = getpid();
    int inInterval = 0;

    for (int i = 0; i <= ITERATIONS; ++i) {
        pid_t tid = pid;

        if (i == 30) {
            sem_wait(sem);
            inInterval = 1;
        }

        char letter = username[i % nameLen];

        printf("PID=%d\tTID=%d\tПроцесс=%s\tИтерация=%d\t%c\n", (int)pid, (int)tid, procName, i, letter);
        fflush(stdout);

        usleep(100000);

        if (i == 60 && inInterval) {
            sem_post(sem);
            inInterval = 0;
        }
    }

    if (inInterval) sem_post(sem);
}

int main(void)
{
    const char* envUser = getenv("USER");
    if (!envUser || envUser[0] == '\0') { fprintf(stderr, "Failed to get USER from environment\n"); return 1; }

    strncpy(username, envUser, sizeof(username) - 1);
    username[sizeof(username) - 1] = '\0';

    nameLen = (int)strlen(username);
    if (nameLen <= 0) { fprintf(stderr, "Username length is 0\n"); return 2; }

    printf("Username: %s,\nLength: %d\n\n", username, nameLen);
    fflush(stdout);

    sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0600, 1);
    if (sem == SEM_FAILED) {
        sem = sem_open(SEM_NAME, 0);
        if (sem == SEM_FAILED) { perror("sem_open failed"); return 3; }
    }

    pid_t pidA = fork();
    if (pidA < 0) { perror("fork A failed"); sem_close(sem); sem_unlink(SEM_NAME); return 4; }
    if (pidA == 0) { work("A"); sem_close(sem); _exit(0); }

    pid_t pidB = fork();
    if (pidB < 0) { perror("fork B failed"); waitpid(pidA, NULL, 0); sem_close(sem); sem_unlink(SEM_NAME); return 5; }
    if (pidB == 0) { work("B"); sem_close(sem); _exit(0); }

    work("MAIN");

    waitpid(pidA, NULL, 0);
    waitpid(pidB, NULL, 0);

    sem_close(sem);
    sem_unlink(SEM_NAME);
    
    return 0;
}
