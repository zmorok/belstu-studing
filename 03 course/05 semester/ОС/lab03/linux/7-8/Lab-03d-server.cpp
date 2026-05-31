#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>     // pipe, fork, dup2, exec
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // waitpid
#include <pthread.h>    // pthreads
#include <cerrno>
#include <cstring>

using namespace std;

struct ChildInfo {
    pid_t  pid = -1;
    int    fd  = -1;
    string note;
};

struct ReaderParam {
    int    fd;
    string tag;
};

static void* ReaderThread(void* p) {
    ReaderParam* rp = static_cast<ReaderParam*>(p);
    const size_t BUF = 4096;
    char buf[BUF];

    while (true) {
        ssize_t got = read(rp->fd, buf, BUF);
        if (got <= 0) break;
        cout << "[" << rp->tag << "] " << string(buf, buf + got);
        cout.flush();
    }

    close(rp->fd);
    delete rp;
    return nullptr;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: Lab-03d-server <procCount> <low> <high>\n";
        return 1;
    }

    long procCount = strtol(argv[1], nullptr, 10);
    long low       = strtol(argv[2], nullptr, 10);
    long high      = strtol(argv[3], nullptr, 10);

    if (procCount <= 0 || low > high || low < 0 || high < 0) {
        cerr << "Invalid arguments\n";
        return 1;
    }

    const char* clientExe = "./Lab-03d-client";

    cout << "Using client exe: " << clientExe << "\n";
    cout << "SERVER PID=" << (int)getpid() << " start, procs=" << procCount << " range=[" << low << "," << high << "]\n";

    long total = high - low + 1;
    long base  = total / procCount;
    long rest  = total % procCount;

    vector<ChildInfo> children;
    children.reserve(procCount);
    vector<pthread_t> threads;

    long curL = low;
    for (long i = 0; i < procCount; ++i) {
        long len = base + (i < rest ? 1 : 0);
        long curH = curL + len - 1;

        int fds[2];
        if (pipe(fds) == -1) { perror("pipe"); break; }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            close(fds[0]);
            close(fds[1]);
            break;
        }
        else if (pid == 0) { // CHILD
            close(fds[0]);  //  -read-end
            if (dup2(fds[1], STDOUT_FILENO) == -1) { perror("dup2 stdout"); _exit(1); }
            if (dup2(fds[1], STDERR_FILENO) == -1) { perror("dup2 stderr"); _exit(1); }
            close(fds[1]);

            string sL = to_string(curL);
            string sH = to_string(curH);

            execl(clientExe, "Lab_03d_client", sL.c_str(), sH.c_str(), (char*)nullptr);

            perror("execl"); // если вернулись сюда, то ошибка
            _exit(127);
        }
        else { // PARENT
            close(fds[1]);   //  -write-end

            ChildInfo ci;
            ci.pid  = pid;
            ci.fd   = fds[0];
            ci.note = "child#" + to_string(i) + " [" + to_string(curL) + "," + to_string(curH) + "]";

            cout << "[OK ] started " << ci.note << " PID=" << (int)pid << "\n";

            ReaderParam* rp = new ReaderParam{ ci.fd, ci.note };
            pthread_t th;
            int rc = pthread_create(&th, nullptr, &ReaderThread, rp);
            if (rc == 0) {
                threads.push_back(th);
                children.push_back(ci);
            } else {
                cerr << "[WRN] reader thread create failed: " << strerror(rc) << "\n";
                close(ci.fd);
                delete rp;
            }
        }

        curL = curH + 1;
    }

    for (auto& c : children) {
        int status = 0;
        if (waitpid(c.pid, &status, 0) == -1) {
            perror("waitpid");
        } else {
            if (WIFEXITED(status)) {
                cout << "[INFO] " << c.note << " exit=" << WEXITSTATUS(status) << "\n";
            } else if (WIFSIGNALED(status)) {
                cout << "[INFO] " << c.note << " killed by signal " << WTERMSIG(status) << "\n";
            }
        }
    }


    for (pthread_t th : threads) {
        pthread_join(th, nullptr);
    }

    cout << "SERVER done.\n";
    return 0;
}
