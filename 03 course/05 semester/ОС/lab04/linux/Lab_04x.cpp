#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>     // getpid, usleep
#include <pwd.h>        // getpwuid
#include <sys/types.h>  // getuid


void* Lab_04x(void* lpParam)
{
    if (lpParam == NULL) { fprintf(stderr, "Lab_04x: lpParam is NULL\n"); return (void*)1; }

    int iterations = *(int*)lpParam;

    const char* envUser = getenv("USER");
    if (!envUser || envUser[0] == '\0') { fprintf(stderr, "Lab_04x: USER env variable not found\n"); return (void*)5; }

    char username[200];
    strncpy(username, envUser, sizeof(username) - 1);
    username[sizeof(username) - 1] = '\0';

    if (iterations <= 0) { fprintf(stderr, "Lab_04x: iterations must be > 0\n"); return (void*)2; }

    int nameLen = (int)strlen(username);
    if (nameLen == 0) { fprintf(stderr, "Lab_04x: username length is 0\n"); return (void*)4; }

    pid_t pid = getpid();
    pthread_t tid = pthread_self();

    for (int i = 0; i < iterations; ++i) {
        char letter = username[i % nameLen];
        printf("PID=%d\t- TID=%lu\t- №Итерации %d\t- %c\n", pid, (unsigned long)tid, i + 1, letter);
        usleep(350 * 1000);
    }

    return 0;
}

int main()
{
    pthread_t thread;
    int iterations = 30;

    int rc = pthread_create(&thread, NULL, Lab_04x, &iterations);

    if (rc != 0) { fprintf(stderr, "Error creating thread: %d\n", rc); return 1; }

    void* threadResult = NULL;
    pthread_join(thread, &threadResult);

    return 0;
}
