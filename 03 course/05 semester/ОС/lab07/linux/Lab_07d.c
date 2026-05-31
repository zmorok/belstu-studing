#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

static pid_t start_child(const char *exe_path, const char *seconds_str)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return -1;
    }

    if (pid == 0)
    {
        execl(exe_path, exe_path, seconds_str, (char*)NULL);

        perror("execl");
        _exit(127);
    }

    return pid;
}

int main(void)
{
    char child_path[PATH_MAX] = "Lab_07x";
    printf("Child exe path: %s\n", child_path);

    pid_t p1 = start_child(child_path, "60");
    if (p1 < 0) return 1;

    pid_t p2 = start_child(child_path, "120");
    if (p2 < 0)
    {
        int st;
        waitpid(p1, &st, 0);
        return 1;
    }

    printf("Запуск дочерних процессов:\n");
    printf("  Дочерний #1 PID=%d (60 sec)\n",  (int)p1);
    printf("  Дочерний #2 PID=%d (120 sec)\n", (int)p2);

    int finished = 0;
    while (finished < 2)
    {
        int status = 0;
        pid_t pid = wait(&status);
        if (pid < 0)
        {
            perror("wait");
            break;
        }

        if (WIFEXITED(status)) printf("Процесс PID=%d завершился, exit code=%d\n", (int)pid, WEXITSTATUS(status));
        else if (WIFSIGNALED(status)) printf("Процесс PID=%d завершился по сигналу %d\n", (int)pid, WTERMSIG(status));
        else printf("Процесс PID=%d завершился (status=0x%x)\n", (int)pid, status);

        finished++;
    }

    return 0;
}