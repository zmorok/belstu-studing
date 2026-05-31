#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>     // fork, execl, getpid, setenv, unsetenv
#include <sys/wait.h>   // waitpid, WIFEXITED...
#include <errno.h>
#include <string.h>     // strerror

using namespace std;

static void perrx(const char* where) {
    cerr << "[ERR] " << where << ": " << strerror(errno) << "\n";
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cerr << "Usage: Lab-03a-server <iter_num>\n";
        return 1;
    }

    long iter_num = strtol(argv[1], nullptr, 10);

    string iters = to_string(iter_num);

    struct ChildRun {
        string note;
        pid_t  pid = -1;
    };
    vector<ChildRun> runs;
    runs.reserve(2);

    // -------- Case #1 --------
    {
        pid_t pid = fork();
        if (pid < 0) perrx("fork (case #1)");
        else if (pid == 0) {

            execlp("konsole", "konsole", "--hold", "-e", "./Lab-03x", iters.c_str() (char*)nullptr);

            perrx("execl Lab-03x (case #1)");
            _exit(127);
        } else {
            cout << "[OK ] Started Case #1 (arg), child PID=" << pid << "(iter_num:" + iters.c_str() + ")" << "\n";
            runs.push_back({"Case #1 (argv)", pid});
        }
    }

    // -------- Case #2 --------
    {
        if (setenv("ITER_NUM", "5", 1) != 0)  perrx("setenv ITER_NUM");

        pid_t pid = fork();
        if (pid < 0) perrx("fork (case #2)");
        else if (pid == 0) {

            execlp("konsole", "konsole", "--hold", "-e", "./Lab-03x", (char*)nullptr);

            perrx("execl Lab-03x (case #2)");
            _exit(127);
        } else {
            cout << "[OK ] Started Case #2 (env), child PID=" << pid << "\n";
            runs.push_back({"Case #2 (env)", pid});
        }

        unsetenv("ITER_NUM");
    }

    // waiting
    int remaining = 0;
    for (auto& r : runs) if (r.pid > 0) ++remaining;

    while (remaining > 0) {
        int status = 0;
        pid_t finished = waitpid(-1, &status, 0);  // ждём любой дочерний
        if (finished < 0) { perrx("waitpid"); break; }
        for (auto& r : runs) {
            if (r.pid == finished) {
                if (WIFEXITED(status)) {
                    cout << "[INFO] " << r.note << " (PID=" << finished
                    << ") exited, code=" << WEXITSTATUS(status) << "\n";
                } else if (WIFSIGNALED(status)) {
                    cout << "[INFO] " << r.note << " (PID=" << finished
                    << ") killed by signal " << WTERMSIG(status) << "\n";
                } else {
                    cout << "[INFO] " << r.note << " (PID=" << finished
                    << ") finished with non-standard status\n";
                }
                --remaining;
                break;
            }
        }
    }

    cout << "[OK ] All child processes finished.\n";
    return 0;
}
