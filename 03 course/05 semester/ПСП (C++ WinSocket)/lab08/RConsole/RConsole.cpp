#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>

char* SetErrorMsgText(const char* msgText, int code)
{
    static char buffer[256];
    sprintf(buffer, "%s %d", msgText, code);
    return buffer;
}

HANDLE ConnectToPipe(const char* pipeName)
{
    HANDLE hPipe;
    while (true)
    {
        hPipe = CreateFileA(
            pipeName,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (hPipe != INVALID_HANDLE_VALUE) break;

        DWORD err = GetLastError();
        if (err == ERROR_PIPE_BUSY)
        {
            printf("Pipe busy, waiting...\n");
            WaitNamedPipeA(pipeName, 2000);
        }
        else if (err == ERROR_FILE_NOT_FOUND)
        {
            printf("Pipe not found, waiting for server...\n");
            Sleep(2000);
        }
        else
        {
            printf("CreateFile failed: %s\n", SetErrorMsgText("", err));
            Sleep(2000);
        }
    }

    printf("Connected to pipe!\n");
    return hPipe;
}

int main(int argc, char* argv[])
{
    char pipeName[256];
    char hostname[256] = "concurrent";
    //char callname[256] = "dualpc";
    char callname[256] = "DESKTOP-E7USIP6";
    if (argc > 1) strcpy(callname, argv[1]);
    if (argc > 2) strcpy(hostname, argv[2]);
    sprintf(pipeName, R"(\\%s\pipe\%s)", callname, hostname);
    printf("=========================================\n");
    printf("               RConsole v1               \n");
    printf("=========================================\n\n");
    printf(R"([RConsole] Using %s)", pipeName);
    printf("\n");
    HANDLE hPipe = ConnectToPipe(pipeName);
    while (true)
    {
        char cmd[256];
        printf("\nEnter command: ");
        if (!fgets(cmd, sizeof(cmd), stdin)) break;

        size_t len = strlen(cmd);
        if (len > 0 && cmd[len - 1] == '\n') cmd[len - 1] = '\0';

        DWORD written = 0;
        BOOL ok = WriteFile(hPipe, cmd, (DWORD)(strlen(cmd) + 1), &written, NULL);
        if (!ok)
        {
            DWORD err = GetLastError();
            printf("WriteFile failed: %s\n", SetErrorMsgText("", err));
            CloseHandle(hPipe);
            printf("Reconnecting...\n");
            hPipe = ConnectToPipe(pipeName);
            continue;
        }

        char rbuf[512] = { 0 };
        DWORD read = 0;
        ok = ReadFile(hPipe, rbuf, sizeof(rbuf) - 1, &read, NULL);
        if (!ok)
        {
            DWORD err = GetLastError();
            printf("ReadFile failed: %s\n", SetErrorMsgText("", err));
            CloseHandle(hPipe);
            printf("Reconnecting...\n");
            hPipe = ConnectToPipe(pipeName);
            continue;
        }

        rbuf[read] = '\0';
        printf("Server response: %s\n", rbuf);
    }

    CloseHandle(hPipe);
    return 0;
}
