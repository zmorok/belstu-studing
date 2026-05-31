#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>

#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")

using namespace std;

static string errmsg(string msg) { return msg + ": " + to_string(GetLastError()); }

int main()
{
    system("chcp 65001 > nul");

    const char* PIPE_NAME = R"(\\dualpc\pipe\Tube)";

    cout << "Введите сообщение для отправки: ";
    string base; getline(cin, base);

    cout << "Количество итераций: ";
    int count = 0; cin >> count;

    try
    {
        long i = 1;

        for (int iter = 0; iter < count; ++iter)
        {
            string out = base + " " + to_string(i);

            char buf[2048];
            DWORD rd = 0;

            BOOL ok = CallNamedPipeA(
                PIPE_NAME,
                (LPVOID)out.c_str(),
                (DWORD)out.size(),
                buf,
                (DWORD)sizeof(buf) - 1,
                &rd,
                NMPWAIT_USE_DEFAULT_WAIT
            );

            if (!ok)
                throw errmsg("CallNamedPipeA | error=");

            buf[rd] = '\0';
            string reply(buf, rd);

            cout << "[Client] sent:  " << out << "\n";
            cout << "[Client] reply: " << reply << "\n";

            int p = (int)reply.find_last_of(' ');
            int num = stoi(reply.substr(p + 1, reply.length() - p - 1));
            i = num + 1;
            
            WaitNamedPipeA(PIPE_NAME, 10);
        }

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
