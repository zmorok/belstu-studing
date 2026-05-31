#include <iostream>
#include <cstdlib>
#include <unistd.h>              // getpid, isatty
#include <thread>                // sleep_for
#include <chrono>                // milliseconds

using namespace std;

int main(int argc, char* argv[]) {
    try {
        long iterations = 0;

        auto parse_positive = [](const char* s, long& out) -> bool {
            if (!s) return false;
            char* end = nullptr;
            long v = strtol(s, &end, 10);
            if (end == s || *end != '\0' || v <= 0) return false;
            out = v;
            return true;
        };

        if (argc > 1) {
            if (!parse_positive(argv[1], iterations)) {
                cout << "Ошибка: аргумент должен быть положительным числом.\n";
                return 1;
            }
        } else {
            const char* env = getenv("ITER_NUM");
            if (!parse_positive(env, iterations)) {
                cout << "Ошибка: не задано количество итераций или неверное значение переменной окружения.\n";
                return 1;
            }
        }

        cout << "Количество итераций: " << iterations << "\n";
        pid_t pid = getpid();

        for (long i = 0; i < iterations; ++i) {
            cout << "PID: " << pid << ", Итерация: " << (i + 1) << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
        }

        cout << "Программа завершена.\n";

        if (isatty(STDIN_FILENO)) {
            cout << "Нажмите Enter для выхода...";
            cout.flush();
            cin.get();
        }

        return 0;
    } catch (const exception& ex) {
        cout << "Произошла непредвиденная ошибка: " << ex.what() << "\n";
        return 1;
    } catch (...) {
        cout << "Произошла непредвиденная ошибка неизвестного типа.\n";
        return 1;
    }
}
