#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>

#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")

using namespace std;

static string errmsg(string msg) { return msg + ": " + to_string(GetLastError()); }

int main() 
{
    try 
    {
        system("chcp 65001 > nul");

        const char* PIPE_NAME = R"(\\.\pipe\Tube)";
        cout << "[Server] starting; pipe=" << PIPE_NAME << "\n";

        while (true) 
        {
            HANDLE hPipe = CreateNamedPipeA(
                PIPE_NAME,
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                PIPE_UNLIMITED_INSTANCES,
                NULL, NULL, NULL, NULL
            );
            if (hPipe == INVALID_HANDLE_VALUE) throw errmsg("CreateNamedPipeA | Cannot create pipe");

            cout << "[Server] waiting for client...\n";
            if (!ConnectNamedPipe(hPipe, nullptr) && GetLastError() != ERROR_PIPE_CONNECTED) {
                CloseHandle(hPipe);
                throw errmsg("ConnectNamedPipe | Cannot get into pipe");
                continue;
            }

            cout << "[Server] client connected\n";

            while (true) 
            {
                char buf[2048];
                DWORD rd = 0;
                if (!ReadFile(hPipe, buf, sizeof(buf) - 1, &rd, nullptr)) {
                    if (GetLastError() == ERROR_BROKEN_PIPE) cout << "[Server] client disconnected\n";
                    else throw errmsg("ReadFile(server) | Cannot read from pipe");
                    break;
                }
                buf[rd] = '\0';
                cout << "[Server] got: " << buf << "\n";

                DWORD wr = 0;
                if (!WriteFile(hPipe, buf, rd, &wr, nullptr)) {
                    throw errmsg("WriteFile(server) | Cannot read from pipe");
                    break;
                }
            }

            FlushFileBuffers(hPipe);
            DisconnectNamedPipe(hPipe);
            CloseHandle(hPipe);
        }

    }
    catch (string err) 
    {
		cout << "[SERVER] Exception: " << err << endl;
        system("pause");
		return 1;
    }
}
