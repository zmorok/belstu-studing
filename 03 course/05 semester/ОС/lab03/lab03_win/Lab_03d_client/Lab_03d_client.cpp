#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

static bool isPrime(unsigned int x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    unsigned int r = (unsigned int)sqrt(x);
    for (unsigned int d = 3; d <= r; d += 2) if (x % d == 0) return false;
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 3) { cerr << "Usage: Lab-03d-client <low> <high>\n"; return 1; }

    long low = strtol(argv[1], nullptr, NULL);
    long high = strtol(argv[2], nullptr, NULL);
    if ( low > high || low < 0 || high < 0) {
        cerr << "Invalid range\n";
        return 1;
    }

    string out;
    DWORD pid = GetCurrentProcessId();
    out += "CLIENT PID=" + to_string(pid) + " RANGE=[" + to_string(low) + "," + to_string(high) + "]\n" + "PRIMES:\n" + ' ';

    bool first = true;
    for (unsigned int v = (unsigned int)low; v <= (unsigned int)high; ++v) {
        if (isPrime(v)) {
            if (!first) out += ' ';
            out += to_string(v);
            first = false;
        }
    }
    out += "\nEND\n";

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) {
        cerr << "No stdout\n";
        return 1;
    }

    DWORD written = 0;
    if (!WriteFile(hStdOut, out.data(), (DWORD)out.size(), &written, nullptr)) return 2;
    
    system("pause");
    CloseHandle(hStdOut);
    return 0;
}