#define _CRT_SECURE_NO_WARNINGS
#define AS_COUNT 10
#include <iostream>
#include <windows.h>
#include <list>
#include "Contact.h"
#pragma comment(lib, "WS2_32.lib")

int port = 2000;
int uport = 2000;
char dllName[256] = "ServiceLibrary.dll";
char hostname[256] = "concurrent";
char callname[256] = "zmorok";
SOCKET sS;
HANDLE(*ts) (char*, LPVOID); // указатель на "динамически экспортированную" функцию из DLL
HMODULE st = LoadLibraryA(dllName);
HANDLE hClientConnectedEvent;
CRITICAL_SECTION scListContact;
ListContact contacts;
volatile long opened = 0;

// количество соединений
volatile long connectionCount = 0;
// количество прерванных соединений
volatile long sayNoCount = 0;
// успешные соединения
volatile long successConnections = 0;
// активные соединения
volatile long currentActiveConnections = 0;

enum TalkersCommand
{
    START,
    STOP,
    EXIT,
    GETCOMMAND,
    WAIT,
    SHUTDOWN
};

HANDLE hAcceptServer = nullptr,
       hDispatchServer = nullptr,
       hConsolePipe = nullptr,
       hGarbageCleaner = nullptr,
       hResponseServer = nullptr;

DWORD WINAPI AcceptServer(LPVOID pPrm);
DWORD WINAPI DispatchServer(LPVOID pPrm);
DWORD WINAPI GarbageCleaner(LPVOID pPrm);
DWORD WINAPI ConsolePipe(LPVOID pPrm);
DWORD WINAPI ResponseServer(LPVOID pPrm);
void CALLBACK ASWTimer(LPVOID Prm, DWORD, DWORD);

void CommandsCycle(TalkersCommand& cmd);
bool AcceptCycle(int count);
bool GetRequestFromClient(const char* expectedRequest, short port, struct sockaddr* from, int* flen);
bool PutAnswerToClient(const char* answer, struct sockaddr* to, int* lto);
char* SetErrorMsgText(const char* msgText, int code);

int main(int argc, char* argv[])
{
    if (argc > 1) port = atoi(argv[1]);
    if (argc > 2) strcpy(dllName, argv[2]);
    if (argc > 3) strcpy(hostname, argv[3]);
    if (argc > 4) uport = atoi(argv[4]);
    if (argc > 5) strcpy(callname, argv[5]);
    printf("=========================================\n");
    printf("         Concurrent Server v1            \n");
    printf("=========================================\n");
    printf(" Configuration:\n");
    printf("   - TCP Port:      %d\n", port);
    printf("   - UDP Port:      %d\n", uport);
    printf("   - Library:       %s\n", dllName);
    printf("   - Hostname:      %s\n", hostname);
    printf("   - Callname:      %s\n", callname);
    printf("=========================================\n\n");
    if (st == NULL)
    {
        throw SetErrorMsgText("LoadLibrary: ", GetLastError());
    }
    ts = (HANDLE (*)(char*, LPVOID))GetProcAddress(st, "SSS");
    volatile TalkersCommand cmd = START;
    InitializeCriticalSection(&scListContact);
    hAcceptServer = CreateThread(NULL, 0, AcceptServer, (LPVOID)&cmd, 0, NULL);
    hDispatchServer = CreateThread(NULL, 0, DispatchServer, (LPVOID)&cmd, 0, NULL);
    hGarbageCleaner = CreateThread(NULL, 0, GarbageCleaner, (LPVOID)&cmd, 0, NULL);
    hConsolePipe = CreateThread(NULL, 0, ConsolePipe, (LPVOID)&cmd, 0,  NULL);
    hResponseServer = CreateThread(NULL, 0, ResponseServer, (LPVOID)&cmd, 0, NULL);

    if (!hAcceptServer) throw SetErrorMsgText("hAcceptServer", GetLastError());
    else SetThreadPriority(hAcceptServer, THREAD_PRIORITY_ABOVE_NORMAL);
  
    if (!hDispatchServer) throw SetErrorMsgText("hDispatchServer", GetLastError());
    else SetThreadPriority(hDispatchServer, THREAD_PRIORITY_HIGHEST);
    
    if (!hGarbageCleaner) throw SetErrorMsgText("hGarbageCleaner", GetLastError());
    else SetThreadPriority(hGarbageCleaner, THREAD_PRIORITY_LOWEST);
    
    if (!hConsolePipe) throw SetErrorMsgText("hConsolePipe", GetLastError());
    else SetThreadPriority(hConsolePipe, THREAD_PRIORITY_BELOW_NORMAL);

    if (!hResponseServer) throw SetErrorMsgText("hResponseServer", GetLastError());
    else SetThreadPriority(hResponseServer, THREAD_PRIORITY_ABOVE_NORMAL);

    WaitForSingleObject(hAcceptServer, INFINITE);
    WaitForSingleObject(hDispatchServer, INFINITE);
    WaitForSingleObject(hGarbageCleaner, INFINITE);
    WaitForSingleObject(hConsolePipe, INFINITE);
    WaitForSingleObject(hResponseServer, INFINITE);

    CloseHandle(hAcceptServer);
    CloseHandle(hDispatchServer);
    CloseHandle(hGarbageCleaner);
    CloseHandle(hConsolePipe);
    CloseHandle(hResponseServer);

    DeleteCriticalSection(&scListContact);
    FreeLibrary(st);
    return 0;
}

DWORD WINAPI AcceptServer(LPVOID pPrm)
{
    WSADATA wsaData;
    hClientConnectedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (!hClientConnectedEvent)
    {
        printf("CreateEvent failed: %d\n", GetLastError());
        ExitThread(1);
    }

    try {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) throw SetErrorMsgText("WSAStartup", WSAGetLastError());

        if ((sS = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) throw SetErrorMsgText("socket:", WSAGetLastError());

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(port);
        serv.sin_addr.s_addr = INADDR_ANY;

        if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR) throw SetErrorMsgText("bind:", WSAGetLastError());
        if (listen(sS, SOMAXCONN) == SOCKET_ERROR) throw SetErrorMsgText("listen:", WSAGetLastError());

        u_long nonblk = 1;
        if (ioctlsocket(sS, FIONBIO, &nonblk) == SOCKET_ERROR)  throw SetErrorMsgText("ioctlsocket:", WSAGetLastError());

        TalkersCommand& cmd = *((TalkersCommand*)pPrm);

        // весь цикл работы сервера, пока не придёт EXIT
        while (cmd != EXIT)
        {
            CommandsCycle(cmd);
            if (cmd == EXIT) break;

            SleepEx(50, TRUE);
        }

        printf("[AcceptServer] Shutting down...\n");

        closesocket(sS);
        WSACleanup();
    }
    catch (const char* message)
    {
        printf("%s\n", message);
    }

    SetEvent(hClientConnectedEvent);
    CloseHandle(hClientConnectedEvent);

    printf("[AcceptServer] ExitThread\n");
    ExitThread(0);
}

DWORD WINAPI DispatchServer(LPVOID pPrm)
{
    // ждёт, пока ацепт создаст "клиент подключился"
    while (hClientConnectedEvent == NULL)
    {
        Sleep(10); 
    }
    DWORD rc = 0;
    TalkersCommand& command = *(TalkersCommand*)pPrm;
    while (command != EXIT)
    {
        if (command != STOP)
        {
            WaitForSingleObjectEx(hClientConnectedEvent, 100, TRUE);
            ResetEvent(hClientConnectedEvent);
            
            EnterCriticalSection(&scListContact);
            time_t now = time(NULL);

             for (auto& contact : contacts)
             {
                 // клиент стоит в простое долго?
                if (contact.type != Contact::EMPTY && now - contact.lastActivityTime > 60)
                {
                    printf("Client timed out\n");
                    send(contact.s, "Timeout due to inactivity", 26, 0);
                    closesocket(contact.s);
                    contact.type = Contact::EMPTY;
                    InterlockedDecrement(&currentActiveConnections);
                    InterlockedIncrement(&sayNoCount);
                    continue;
                }

                // клиент подключен, но его запрос не обработан, сейчас будет обрабатываться
                if (contact.type == contact.ACCEPT)
                {
                    char message[256];
                    int r = recv(contact.s, message, sizeof(message), 0);

                    if (r == SOCKET_ERROR)
                    {
                        if (WSAGetLastError() == WSAEWOULDBLOCK) continue;
                        printf("recv error: %d\n", WSAGetLastError());
                        InterlockedIncrement(&sayNoCount);
                        closesocket(contact.s);
                        contact.type = Contact::EMPTY;
                        continue;
                    }
                    else if (r == 0)
                    {
                        closesocket(contact.s);
                        contact.type = Contact::EMPTY;
                        continue;
                    }

                    message[r] = '\0';
                    contact.lastActivityTime = now;

                    // валидная команда - обрабатываем
                    if (strcmp(message, "echo") == 0 || strcmp(message, "time") == 0 || strcmp(message, "rand") == 0)
                    {
                        printf("Request received: %s\n", message);
                        if (send(contact.s, "Request received", strlen("Request received") + 1, 0) == SOCKET_ERROR)
                        {
                            printf("send error: %d\n", WSAGetLastError());
                            closesocket(contact.s);
                            contact.type = Contact::EMPTY;
                            continue;
                        }

                        contact.type = Contact::CONTACT;
                        u_long blocking = 0;
                        LARGE_INTEGER Li;
                        int seconds = 60;
                        Li.QuadPart = -(10000000 * seconds);
                        ioctlsocket(contact.s, FIONBIO, &blocking);
                        contact.acceptserver = &hAcceptServer;
                        contact.hthread = ts(message, &contact);
                        contact.htimer = CreateWaitableTimer(0, FALSE, 0);
                        strcpy(contact.srvname, message);
                        SetWaitableTimer(contact.htimer, &Li, 0, ASWTimer, (LPVOID) & (contact), FALSE);
                        SleepEx(0, TRUE);
                    }
                    else
                    {
                        send(contact.s, "ErrorInquiry", strlen("ErrorInquiry") + 1, 0);
                        contact.sthread = Contact::ABORT;
                        contact.type = Contact::EMPTY;
                        InterlockedIncrement(&sayNoCount);
                        closesocket(contact.s);
                        InterlockedDecrement(&currentActiveConnections);
                    }
                }
            }

            LeaveCriticalSection(&scListContact);
        }

        SleepEx(100, TRUE);
    }

    ExitThread(*(DWORD*)pPrm);
}

DWORD WINAPI GarbageCleaner(LPVOID pPrm)
{
    TalkersCommand& cmd = *(TalkersCommand*)pPrm;
    while (cmd != EXIT)
    {
        EnterCriticalSection(&scListContact);

        for (auto it = contacts.begin(); it != contacts.end(); )
        {
            if (it->type == Contact::EMPTY)
            {
                if (it->sthread == Contact::FINISH)
                {
                    InterlockedIncrement(&successConnections);
                    InterlockedDecrement(&currentActiveConnections);
                }
                else if (it->sthread == Contact::WORK)
                {
                    InterlockedDecrement(&currentActiveConnections);
                }
                if (it->s != INVALID_SOCKET)
                {
                    closesocket(it->s);
                    it->s = INVALID_SOCKET;
                }
                if (it->htimer)
                {
                    CancelWaitableTimer(it->htimer);
                    CloseHandle(it->htimer);
                    it->htimer = nullptr;
                }
                if (it->hthread)
                {
                    CloseHandle(it->hthread);
                    it->hthread = nullptr;
                }
                it = contacts.erase(it);
                continue;
            }
            ++it;
        }

        LeaveCriticalSection(&scListContact);
        Sleep(2000);
    }

    printf("[GarbageCleaner] ExitThread\n");
    ExitThread(0);
}

DWORD WINAPI ConsolePipe(LPVOID pPrm)
{
    TalkersCommand* pCmd = (TalkersCommand*)pPrm;
    char pipeName[256];
    sprintf(pipeName, "\\\\.\\pipe\\%s", hostname);

    HANDLE hPipe = CreateNamedPipeA(
        pipeName,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1,
        1024, 1024,
        0,
        NULL
    );

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        printf("CreateNamedPipe failed: %d\n", GetLastError());
        ExitThread(1);
    }

    printf("[ConsolePipe] Waiting for client on %s...\n", pipeName);

    BOOL connected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

    if (!connected)
    {
        printf("ConnectNamedPipe failed: %d\n", GetLastError());
        CloseHandle(hPipe);
        ExitThread(1);
    }

    printf("[ConsolePipe] Client connected!\n");

    char buffer[256];
    DWORD bytesRead = 0;

    while (true)
    {
        bool disconnectAfterWrite = false;
        BOOL ok = ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
        if (!ok || bytesRead == 0)
        {
            printf("[ConsolePipe] Client disconnected.\n");
            DisconnectNamedPipe(hPipe);
            ConnectNamedPipe(hPipe, NULL);
            continue;
        }
        buffer[bytesRead] = '\0';
        printf("[ConsolePipe] Received: %s\n", buffer);
        for (char* p = buffer; *p; ++p) *p = tolower(*p);

        const char* response = "Unknown command";
        bool known = true;

        if (strcmp(buffer, "start") == 0)
        {
            *pCmd = START;
            response = "Server started";
        }
        else if (strcmp(buffer, "stop") == 0)
        {
            *pCmd = STOP;
            response = "Server stoped";
        }
        else if (strcmp(buffer, "shutdown") == 0)
        {
            *pCmd = SHUTDOWN;
            response = "Shutdown initiated";
            disconnectAfterWrite = true;
        }
        else if (strcmp(buffer, "help") == 0)
        {
            response = "API commands: start stop wait shutdown status help exit";
        }
        else if (strcmp(buffer, "wait") == 0)
        {
            *pCmd = WAIT;
            response = "Waiting for clients...";
        }
        else if (strcmp(buffer, "exit") == 0)
        {
            *pCmd = EXIT;
            response = "Exiting...";
        }
        else if (strcmp(buffer, "status") == 0 || strcmp(buffer, "statistics") == 0)
        {
            extern volatile long connectionCount;
            extern volatile long sayNoCount;
            extern volatile long successConnections;

            size_t active = 0;
            EnterCriticalSection(&scListContact);
            active = contacts.size();
            LeaveCriticalSection(&scListContact);

            char statbuf[256];
            sprintf(statbuf,
                "connectionCount=%ld; sayNoCount=%ld; successConnections=%ld; active=%zu",
                connectionCount,
                sayNoCount,
                successConnections,
                active
            );

            response = statbuf;

            printf("[ConsolePipe] STAT: %s\n", statbuf);
        }
        else
        {
            known = false;
            *pCmd = GETCOMMAND;
        }

        DWORD written;
        WriteFile(hPipe, response, strlen(response) + 1, &written, NULL);

        if (disconnectAfterWrite)
        {
            printf("[ConsolePipe] SHUTDOWN received — console input disabled.\n");
            break;
        }

        if (!known)
        {
            printf("[ConsolePipe] Unknown command, sent 'nocmd'.\n");
            continue;
        }
        if (*pCmd != EXIT && *pCmd != GETCOMMAND)
        {
            while (*pCmd != GETCOMMAND)
                Sleep(100);
        }

        if (*pCmd == EXIT)
        {
            printf("[ConsolePipe] Exit command received — shutting down.\n");
            break;
        }
    }
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
    ExitThread(0);
}

DWORD WINAPI ResponseServer(LPVOID pPrm)
{
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    DWORD rc = 0;
    SOCKADDR_IN from;
    int lc = sizeof(from);
    ZeroMemory(&from, lc);
    int numberOfClients = 0;

    const char* REQUEST_TEXT = "DISCOVER_SERVER_REQUEST";
    const char* RESPONSE_TEXT = "DISCOVER_SERVER_RESPONSE";

    while (*(TalkersCommand*)pPrm != EXIT)
    {
        try
        {
            if (GetRequestFromClient((char*)REQUEST_TEXT, uport, (sockaddr*)&from, &lc))
            {
                if (opened)
                {
                    printf("\nconnected Client: %d, port: %d, address: %s\n", ++numberOfClients, ntohs(from.sin_port), inet_ntoa(from.sin_addr));
                }

                if (!PutAnswerToClient((char*)RESPONSE_TEXT, (sockaddr*)&from, &lc))
                    if (opened) printf("Failed to send response to %s\n", inet_ntoa(from.sin_addr));
            }
        }
        catch (const char* message)
        {
            printf("%s %s\n", "An error occured!", message);
        }
    }

    ExitThread(rc);
}

void CommandsCycle(TalkersCommand& cmd)
{
    int count = 0;
    bool allowConnections = false;
    bool waitForLastClient = false;

    while (cmd != EXIT)
    {
        switch (cmd)
        {
        case START:
            printf("[CommandsCycle] START\n");
            allowConnections = true;
            count = AS_COUNT;
            cmd = GETCOMMAND;
            break;

        case STOP:
            printf("[CommandsCycle] STOP\n");
            allowConnections = false;
            count = 0;
            cmd = GETCOMMAND;
            break;

        case WAIT:
            printf("[CommandsCycle] WAIT\n");
            waitForLastClient = true;
            while (true)
            {
                EnterCriticalSection(&scListContact);
                bool empty = contacts.empty();
                LeaveCriticalSection(&scListContact);
                if (empty) break;
                SleepEx(100, TRUE);
            }
            printf("[CommandsCycle] WAIT COMPLETED\n");
            waitForLastClient = false;
            cmd = GETCOMMAND;
            break;

        case SHUTDOWN:
            printf("[CommandsCycle] SHUTDOWN\n");
            allowConnections = false;
            count = 0;
            while (true)
            {
                EnterCriticalSection(&scListContact);
                bool empty = contacts.empty();
                LeaveCriticalSection(&scListContact);
                if (empty) break;
                SleepEx(100, TRUE);
            }
            printf("[CommandsCycle] SHUTDOWN: all clients finished, exiting\n");
            cmd = EXIT;
            continue;

        case EXIT:
            printf("[CommandsCycle] EXIT requested\n");
            break;

        default:
            SleepEx(50, TRUE);
            break;
        }

        if (allowConnections && cmd != EXIT) { if (AcceptCycle(count)) cmd = GETCOMMAND; }
        else SleepEx(50, TRUE);
    }

    printf("[CommandsCycle] EXIT complete. Closing all clients...\n");
    EnterCriticalSection(&scListContact);
    for (auto& c : contacts) if (c.s != INVALID_SOCKET) closesocket(c.s);
    contacts.clear();
    LeaveCriticalSection(&scListContact);

    printf("[CommandsCycle] Finished. Exiting thread.\n");
}

bool AcceptCycle(int count)
{
    bool rc = false;
    Contact c(Contact::ACCEPT, "EchoServer");
    while (count-- > 0 && rc == false)
    {
        if ((c.s = accept(sS, (sockaddr*)&c.prms, &c.lprms)) == INVALID_SOCKET)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                InterlockedIncrement(&sayNoCount);
                throw SetErrorMsgText("accept:", WSAGetLastError());
            }
                
        }
        else
        {
            rc = true;
            u_long nonblock = 1;
            ioctlsocket(c.s, FIONBIO, &nonblock);

            c.connectTime = time(NULL);
            c.lastActivityTime = time(NULL);

            EnterCriticalSection(&scListContact);
            contacts.push_front(c);
            LeaveCriticalSection(&scListContact);
            SetEvent(hClientConnectedEvent);
            InterlockedIncrement(&currentActiveConnections);
            InterlockedIncrement(&connectionCount);
        }
    }
    return rc;
}

void CALLBACK ASWTimer(LPVOID Prm, DWORD, DWORD) {
    Contact* contact = (Contact*)(Prm);
    if (!contact) return;

    InterlockedIncrement(&sayNoCount);
    InterlockedDecrement(&currentActiveConnections);

    printf("ASWTimer is calling %p\n", contact->acceptserver);

    if (contact->hthread && WaitForSingleObject(contact->hthread, 0) == WAIT_TIMEOUT)
        TerminateThread(contact->hthread, 0);

    if (contact->s != INVALID_SOCKET)
    {
        send(contact->s, "TimeOUT", strlen("TimeOUT") + 1, 0);
        closesocket(contact->s);
        contact->s = INVALID_SOCKET;
    }

    if (contact->htimer) CancelWaitableTimer(contact->htimer);
    contact->type = Contact::EMPTY;
    contact->sthread = Contact::TIMEOUT;
}

bool GetRequestFromClient(const char* name, short port, struct sockaddr* from, int* flen)
{
    SOCKET sS;
    if ((sS = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) throw SetErrorMsgText("GetRequestFromClient | socket:", WSAGetLastError());

    SOCKADDR_IN serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    //serv.sin_addr.s_addr = inet_addr("10.20.89.130");
    //serv.sin_addr.s_addr = inet_addr("192.168.122.1");

    /*
        inet_addr("192.168.122.2");
        inet_addr("192.168.122.3"); no_detect_duplicate
        inet_addr("192.168.122.4"); no_detect_duplicate
    */

    /*std::cout << "\nServer started at ip: " << inet_ntoa(serv.sin_addr) << ":" << ntohs(serv.sin_port) << "\n\n\n";*/

    //printf("\nServer started at ip: %s:%d \n\n\n", inet_ntoa(serv.sin_addr), ntohs(serv.sin_port));

    if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR) throw SetErrorMsgText("GetRequestFromClient | bind:", WSAGetLastError());

    char msgfrom[256];
    SOCKADDR_IN temp;
    int recvSize = 0;

    if ((recvSize = recvfrom(sS, msgfrom, (int)sizeof(msgfrom) - 1, 0, (LPSOCKADDR)&temp, flen)) == SOCKET_ERROR)
    {
        closesocket(sS);
        return false;
    }
    msgfrom[recvSize] = '\0';

    //strcpy(income_echo, msgfrom);

    if (strcmp(msgfrom, name) == 0)
    {
        memcpy(from, &temp, sizeof(SOCKADDR_IN));
        closesocket(sS);
        return true;
    }

    closesocket(sS);
    return false;
}

//bool GetRequestFromClient(const char* expectedRequest, short port, struct sockaddr* from, int* flen)
//{
//    SOCKET sS;
//    if ((sS = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) throw SetErrorMsgText("socket:", WSAGetLastError());
//
//    BOOL reuse = TRUE;
//    setsockopt(sS, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));
//
//    SOCKADDR_IN serv;
//    serv.sin_family = AF_INET;
//    serv.sin_port = htons(port);
//    serv.sin_addr.s_addr = INADDR_ANY;
//
//    if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR) {
//        int err = WSAGetLastError();
//        closesocket(sS);
//        throw SetErrorMsgText("bind:", err);
//    }
//
//    u_long mode = 1;
//    if (ioctlsocket(sS, FIONBIO, &mode) == SOCKET_ERROR) {
//        int err = WSAGetLastError();
//        closesocket(sS);
//        throw SetErrorMsgText("ioctlsocket:", err);
//    }
//
//    char msgfrom[256];
//    SOCKADDR_IN temp;
//    int recvSize;
//
//    DWORD start = GetTickCount64();
//    while (true)
//    {
//        if ((recvSize = recvfrom(sS, msgfrom, sizeof(msgfrom) - 1, 0, (LPSOCKADDR)&temp, flen)) == SOCKET_ERROR)
//        {
//            if (WSAGetLastError() == WSAEWOULDBLOCK)
//            {
//                if (GetTickCount64() - start > 5000)
//                {
//                    closesocket(sS);
//                    return false; 
//                }
//                Sleep(50);
//                continue;
//            }
//            else
//            {
//                closesocket(sS);
//                throw SetErrorMsgText("recvfrom:", WSAGetLastError());
//            }
//        }
//        else
//        {
//            msgfrom[recvSize] = '\0';
//            if (strcmp(msgfrom, expectedRequest) == 0)
//            {
//                memcpy(from, &temp, sizeof(SOCKADDR_IN));
//                closesocket(sS);
//                return true;
//            }
//        }
//    }
//}


bool PutAnswerToClient(const char* answer, struct sockaddr* to, int* lto)
{
    SOCKET scS;
    if ((scS = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) throw SetErrorMsgText("socket:", WSAGetLastError());

    BOOL bcast = TRUE;
    setsockopt(scS, SOL_SOCKET, SO_BROADCAST, (char*)&bcast, sizeof(bcast));

    u_long mode = 1;
    if (ioctlsocket(scS, FIONBIO, &mode) == SOCKET_ERROR) { closesocket(scS); throw SetErrorMsgText("ioctlsocket:", WSAGetLastError()); }

    int sendSize;
    DWORD start = GetTickCount64();

    while (true)
    {
        if ((sendSize = sendto(scS, answer, (int)strlen(answer), 0, to, *lto)) == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAEWOULDBLOCK)
            {
                if (GetTickCount64() - start > 3000)
                {
                    closesocket(scS);
                    return false; 
                }
                Sleep(50);
                continue;
            }
            else { closesocket(scS); throw SetErrorMsgText("sendto:", WSAGetLastError()); }
        }
        else { closesocket(scS); return true; }
    }
}


char* SetErrorMsgText(const char* msgText, int code)
{
    static char buffer[256];
    sprintf(buffer, "%s %d", msgText, code);
    return buffer;
}
