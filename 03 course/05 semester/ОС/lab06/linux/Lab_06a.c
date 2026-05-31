#define _GNU_SOURCE
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define ITERATIONS 90

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

char username[256];
int  nameLen = 0;

typedef struct ThreadParams {
    const char* name;
} ThreadParams;

void* ThreadFunc(void* arg)
{
    ThreadParams* params = (ThreadParams*)arg;
    const char* threadName = params->name;

    pid_t pid = getpid();
    int inInterval = 0;

    for (int i = 0; i <= ITERATIONS; i++) {
        pid_t tid = gettid();

        if (i == 30) {
            pthread_mutex_lock(&mutex);
            inInterval = 1;
        }

        char letter = username[i % nameLen];

        printf("PID=%d\tTID=%lu\tПоток=\t%s\tИтерация=%d\t%c\n", pid, tid, threadName, i, letter);
        fflush(stdout);
        usleep(100000);

        if (i == 60 && inInterval) {
            pthread_mutex_unlock(&mutex);
            inInterval = 0;
        }
    }

    if (inInterval) {
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
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

    ThreadParams paramsA    = { "A" };
    ThreadParams paramsB    = { "B" };
    ThreadParams paramsMain = { "MAIN" };

    pthread_t threads[2];

    if (pthread_create(&threads[0], NULL, ThreadFunc, &paramsA) != 0) {
        perror("pthread_create(A) failed");
        return 3;
    }

    if (pthread_create(&threads[1], NULL, ThreadFunc, &paramsB) != 0) {
        perror("pthread_create(B) failed");
        return 4;
    }

    ThreadFunc(&paramsMain);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}
