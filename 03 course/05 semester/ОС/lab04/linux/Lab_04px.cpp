#include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>     // getpid, usleep
#include <sys/types.h>  // pid_t
#include <cerrno>

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
        printf("PID=%d\t- TID=%lu\t- №Итерация %d\t- %c\n", pid, (unsigned long)tid, i + 1, letter);
        fflush(stdout);
        usleep(350 * 1000);
    }

    return 0;
}

int main()
{
    int it1 = 50, it2 = 125, itMain = 100;

    pthread_t threads[2];

    // threads[0]
    int rc = pthread_create(&threads[0], nullptr, Lab_04x, &it1);
    if (rc != 0) { fprintf(stderr, "Error creating thread 1: %s\n", strerror(rc)); return 1;}

    // threads[1]
    rc = pthread_create(&threads[1], nullptr, Lab_04x, &it2);
    if (rc != 0) { fprintf(stderr, "Error creating thread 2: %s\n", strerror(rc)); pthread_join(threads[0], nullptr); return 1; }

    Lab_04x(&itMain);

    // waiting threads[0]
    void* threadResult = nullptr;
    rc = pthread_join(threads[0], &threadResult);
    if (rc != 0) { fprintf(stderr, "Error joining thread 1: %s\n", strerror(rc)); }

    // waiting threads[1]
    rc = pthread_join(threads[1], &threadResult);
    if (rc != 0) { fprintf(stderr, "Error joining thread 2: %s\n", strerror(rc)); }

    return 0;
}
