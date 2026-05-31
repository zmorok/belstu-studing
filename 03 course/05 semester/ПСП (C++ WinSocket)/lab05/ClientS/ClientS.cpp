#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <Winsock2.h>
#pragma comment(lib, "WS2_32.lib")

#include "SetErrorMsgText.h"
#include "SetupConsole.h"

using namespace std;

bool GetServer(char* call, short port, struct sockaddr* from, int* flen);
bool GetServerByName(char* name, char* call, struct sockaddr* from, int* flen);

int main()
{
    SetupConsoleLocale();

    WSADATA wsdata;
    SOCKADDR_IN from;

    try
    {
        if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0) throw SetErrorMsgText("WSAStartup:", WSAGetLastError());

        string buf;
        string servname;

        int flen = sizeof(from);

        while (true)
        {
            cout << string(40, '=') << endl;
            cout << "Name: " << endl;
            getline(cin, buf);

            cout << "Servname: " << endl;
            getline(cin, servname);

            int result = GetServerByName((char *)servname.c_str(), (char *)buf.c_str(), (LPSOCKADDR)&from, &flen);
            
            if (!result) {
                cout << "Server not found (timeout)\n";
                continue;
            }

			string message = "\n Response from " + (string)inet_ntoa(from.sin_addr) + ":" + to_string(ntohs(from.sin_port)) + '\n';

            cout << message << endl;
        }
    }
    catch (string message)
    {
        cout << " ERROR: " << message << endl;
    }

    WSACleanup();
    system("pause");
}

bool GetServer(char* call, short port, struct sockaddr* from, int* flen)
{
    SOCKET cC;
    if ((cC = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) throw SetErrorMsgText("GetServer | socket:", WSAGetLastError());

    int optval = 1;
    if (setsockopt(cC, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR) throw SetErrorMsgText("opt:", WSAGetLastError());

    char buf[256];

    SOCKADDR_IN all;
    all.sin_family = AF_INET;
    all.sin_port = htons(2000);
    all.sin_addr.s_addr = inet_addr("192.168.122.255");

    DWORD timeout = 10 * 1000;
    setsockopt(cC, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

    int send = 0;
    if ((send = sendto(cC, call, (int)sizeof(call), 0, (LPSOCKADDR)&all, sizeof(all))) == SOCKET_ERROR) throw SetErrorMsgText("GetServer | sendto:", WSAGetLastError());
    while (true)
    {
        int recv = 0;
        if ((recv = recvfrom(cC, buf, (int)sizeof(buf) - 1, 0, from, flen)) == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT) { closesocket(cC); return false; }
            throw SetErrorMsgText("GetServer | GetRequestFromClient: ", WSAGetLastError());
        }
        buf[recv] = '\0';
        cout << " Ответ от сервера: " << buf << endl;
        if (strcmp(buf, call) == 0)
        {
            cout << " Получен запрос : " << buf << endl;
            closesocket(cC);
            return true;
        }
    }
}

bool GetServerByName(char* name, char* call, struct sockaddr* from, int* flen)
{
    SOCKET cC;
    if ((cC = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    throw SetErrorMsgText("GetServerByName | socket:", WSAGetLastError());

    DWORD timeout = 10 * 1000;
    if (setsockopt(cC, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == SOCKET_ERROR) 
    throw SetErrorMsgText("GetServerByName | setsockopt (timeout):", WSAGetLastError());

    hostent* host = gethostbyname(name);
    if (!host) { closesocket(cC); throw SetErrorMsgText("GetServerByName | gethostbyname:", WSAGetLastError()); }
    else  cout << " Найден хост: " << (host ? host->h_name : "<unknown>") << endl;
    
    SOCKADDR_IN server;
    server.sin_family = AF_INET;
    server.sin_port = htons(2000);
    memcpy(&server.sin_addr, host->h_addr, host->h_length);

    char buf[256];

	cout << "отправка call : \"" + (string)call + "\" на сервер " + (string)inet_ntoa(server.sin_addr) + ":" + to_string(ntohs(server.sin_port)) << endl;
    int send = 0;
    if ((send = sendto(cC, call, strlen(call), 0, (LPSOCKADDR)&server, sizeof(server))) == SOCKET_ERROR)
    { closesocket(cC); throw SetErrorMsgText("GetServerByName | sendto:", WSAGetLastError()); }

    while (true)
    {
        int recv = 0;
        if ((recv = recvfrom(cC, buf, (int)sizeof(buf) - 1, 0, from, flen)) == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT) { closesocket(cC); return false; }
            throw SetErrorMsgText("recvfrom:", WSAGetLastError());
        }
        buf[recv] = '\0';
        cout << " Ответ от сервера: " << buf << endl;
        if (strcmp(buf, call) == 0)
        {
            cout << " Получен запрос : " << buf << endl;
            closesocket(cC);
            return true;
        }
    }
}
