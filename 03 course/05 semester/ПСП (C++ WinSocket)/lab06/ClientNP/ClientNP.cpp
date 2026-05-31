#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>

#include <WinSock2.h>
#pragma comment(lib, "WS2_32.LIB")

using namespace std;

static string errmsg(string msg) { return msg + ": " + to_string(GetLastError()); }

int main() {
	system("chcp 65001 > nul");

    const char* PIPE_NAME = R"(\\dualpc\pipe\Tube)";

    cout << "Введите сообщение для отправки: ";
    string base; getline(cin, base);

    cout << "Количество итераций: ";
	int count = 0; cin >> count;

    try 
    {
        if (!WaitNamedPipeA(PIPE_NAME, NULL))
            throw errmsg("WaitNamedPipeA | Cannot open pipe");

        HANDLE hPipe = CreateFileA(
            PIPE_NAME, 
            GENERIC_READ | GENERIC_WRITE, 
            0, 0, 
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, 
            0
        );
        if (hPipe == INVALID_HANDLE_VALUE) throw errmsg("CreateFileA | Cannot connect to pipe");

        DWORD mode = PIPE_READMODE_MESSAGE;
        if (!SetNamedPipeHandleState(hPipe, &mode, 0, 0)) 
        {
            CloseHandle(hPipe);
            throw errmsg("SetNamedPipeHandleState | Cannot change to listening");
        }

        long i = 1;

        for (int iter = 0; iter < count; ++iter) {
            string out = base + " " + to_string(i);

            DWORD wr = 0;
            if (!WriteFile(hPipe, out.c_str(), (DWORD)out.size(), &wr, NULL)) {
                CloseHandle(hPipe);
                throw errmsg("WriteFile(client->server) | Cannot write to pipe");
            }
            cout << "[Client] sent: " << out << endl;

            char buf[2048];
            DWORD rd = 0;
            if (!ReadFile(hPipe, buf, sizeof(buf) - 1, &rd, NULL)) {
                CloseHandle(hPipe);
                throw errmsg("ReadFile(server->client) | Cannot read from pipe");
            }
            buf[rd] = '\0';
            string reply(buf);
            cout << "[Client] reply: " << reply << endl;

            string m = (string)reply;
            int p = m.find_last_of(' ');
            int num = stoi(m.substr(p + 1, m.length() - p - 1));
            i = ++num;
        }

        FlushFileBuffers(hPipe);
        CloseHandle(hPipe);

        cout << "[Client] done " << count << " messages.\n";
        return 0;
    }
    catch (string err) 
    {
        cerr << "[Client] Exception: " << err << "\n";
        system("pause");
        return 1;
	}
}
