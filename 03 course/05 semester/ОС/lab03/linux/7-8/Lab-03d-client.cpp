#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <unistd.h>     // getpid, write
#include <sys/types.h>  // pid_t

using namespace std;

static bool isPrime(unsigned int x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    unsigned int r = (unsigned int)sqrt((double)x);
    for (unsigned int d = 3; d <= r; d += 2)
        if (x % d == 0) return false;
        return true;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: Lab-03d-client <low> <high>\n";
        return 1;
    }

    long low  = strtol(argv[1], nullptr, 10);
    long high = strtol(argv[2], nullptr, 10);
    if (low > high || low < 0 || high < 0) {
        cerr << "Invalid range\n";
        return 1;
    }

    string out;
    pid_t pid = getpid();
    out += "CLIENT PID=" + to_string((int)pid) +
    " RANGE=[" + to_string(low) + "," + to_string(high) + "]\n"
    "PRIMES:\n" + ' ';

    bool first = true;
    for (unsigned int v = (unsigned int)low; v <= (unsigned int)high; ++v) {
        if (isPrime(v)) {
            if (!first) out += ' ';
            out += to_string(v);
            first = false;
        }
    }
    out += "\nEND\n";

    ssize_t written = write(STDOUT_FILENO, out.data(), out.size());
    if (written < 0) {
        perror("write");
        return 2;
    }

    return 0;
}
