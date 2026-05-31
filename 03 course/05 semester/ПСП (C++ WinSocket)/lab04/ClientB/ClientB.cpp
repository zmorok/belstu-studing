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
            cout << "Name: " << endl;
            getline(cin, buf);

            int result = GetServer((char*)buf.c_str(), 2000, (LPSOCKADDR)&from, &flen);
            if (!result) {
                cout << "Server not found (timeout)\n";
                continue;
            }
            hostent* hostname = gethostbyaddr((char*)&from.sin_addr, sizeof(from.sin_addr), AF_INET);

            cout << "\nServer found at " << inet_ntoa(from.sin_addr) << ":" << ntohs(from.sin_port) << "\nHostname: " << (hostname ? hostname->h_name : "<unknown>") << endl;
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
            if (WSAGetLastError() == WSAETIMEDOUT) { closesocket(cC); return false;}
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