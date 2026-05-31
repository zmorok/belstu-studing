#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <cstdlib>
using namespace std;
int main(int argc, char* argv[])
{
    try 
    {
        int iterations = 0;
        if (argc > 1)
        {
            iterations = atoi(argv[1]);
            if (iterations <= 0) { cout << "Ошибка: аргумент должен быть положительным числом.\n"; ExitProcess(1); }
        }
        else
        {
            char* env = getenv("ITER_NUM");
            if (!env) { cout << "Ошибка: не задано количество итераций.\n"; ExitProcess(1); }
            iterations = atoi(env);
            if (iterations <= 0) { cout << "Ошибка: неверное значение переменной окружения.\n"; ExitProcess(1); }
        }
        cout << "Количество итераций: " << iterations << "\n";
        DWORD pid = GetCurrentProcessId();
        for (int i = 0; i < iterations; i++) { cout << "PID: " << pid << ", Итерация: " << i + 1 << endl; Sleep(500); }
        cout << "Программа завершена.\n";
        system("pause");
        return 0;
    }
    catch (string ex)
    {
        cout << "Произошла непредвиденная ошибка: " << ex << endl;
		system("pause");
        return 1;
	}
}