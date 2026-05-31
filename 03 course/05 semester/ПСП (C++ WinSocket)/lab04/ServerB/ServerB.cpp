#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")

#include "SetErrorMsgText.h"
#include "SetupConsole.h"

using namespace std;

bool GetRequestFromClient(char* name, short port, struct sockaddr* from, int* flen);
bool PutAnswerToClient(char* name, struct sockaddr*, int* lto);
bool CheckForOtherServers(char* name, short port);

int main()
{
    SetupConsoleLocale();

    WSADATA wsdata;
    SOCKADDR_IN from;

    char name[] = "Hello";
    int flen = sizeof(from);

    try
    {
        if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0) throw SetErrorMsgText("WSAStartup", WSAGetLastError());

        if (CheckForOtherServers(name, 2000)) cout << "[ WARNING ] Обнаружены другие сервера с таким же позывным.\n\n";
        
        while (true)
        {
            if (GetRequestFromClient(name, 2000, (LPSOCKADDR)&from, &flen))
            {
               

                string msg = "# Address = " + (string)inet_ntoa(from.sin_addr) + ":" + to_string(ntohs(from.sin_port)) + '\n';

                cout << "Client connected:" << msg;
				cout << "\nSending answer to client... (" + (string)name + ")\n ";

                if (PutAnswerToClient(name, (LPSOCKADDR)&from, &flen)) cout << "+ Answer sent to client\n";
				else cout << "! Failed to send answer to client\n";
            }
            else {
				cout << "No clients requests\n";
            }
        }
    }
    catch (string message)
    {
        cout << " ERROR: " << message << endl;
    }

    WSACleanup();
    return 0;
}


bool GetRequestFromClient(char* name, short port, struct sockaddr* from, int* flen)
{
    SOCKET sS;
    if ((sS = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) throw SetErrorMsgText("GetRequestFromClient | socket:", WSAGetLastError());

    SOCKADDR_IN serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = inet_addr("10.106.20.130");

    /*
        inet_addr("192.168.122.2");
        inet_addr("192.168.122.3"); no_detect_duplicate
        inet_addr("192.168.122.4"); no_detect_duplicate
    */

    cout << "\nServer started at ip: " << inet_ntoa(serv.sin_addr) << ":" << ntohs(serv.sin_port) << endl;

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

    if (strcmp(msgfrom, name) == 0)
    {
        memcpy(from, &temp, sizeof(SOCKADDR_IN));
        closesocket(sS);
        return true;
    }

    closesocket(sS);
    return false;
}

bool PutAnswerToClient(char* name, struct sockaddr* to, int* lto)
{
    SOCKET scS;
    if ((scS = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) throw SetErrorMsgText("PutAnswerToClient | socket:", WSAGetLastError());

    int sendSize = 0;
    if ((sendSize = sendto(scS, name, (int)strlen(name), 0, to, *lto)) == SOCKET_ERROR) throw SetErrorMsgText("PutAnswerToClient | send:", WSAGetLastError());

    closesocket(scS);
    return true;
}

bool CheckForOtherServers(char* name, short port)
{
    SOCKET chS;
    if ((chS = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) throw SetErrorMsgText("CheckForOtherServers | socket:", WSAGetLastError());

    int optval = 1;
    DWORD timeout = 10 * 1000;
    setsockopt(chS, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval));
    setsockopt(chS, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

    SOCKADDR_IN all;
    all.sin_family = AF_INET;
    all.sin_port = htons(port);
    all.sin_addr.s_addr = inet_addr("192.168.122.255");

    sendto(chS, name, (int)strlen(name), 0, (LPSOCKADDR)&all, sizeof(all));

    char buf[256];
    SOCKADDR_IN from{};
    int flen = sizeof(from);
    bool found = false;
    int i = 1;
    while (true)
    {
        int recv = 0;
        if ((recv = recvfrom(chS, buf, sizeof(buf) - 1, 0, (LPSOCKADDR)&from, &flen)) == SOCKET_ERROR) break;
		cout << "Response from someone: " << i++ << " :" << buf << endl;
        buf[recv] = '\0';

        if (strcmp(buf, name) == 0)
        {
			string message = "[Duplicate server] : IP Address = " 
                + (string)inet_ntoa(from.sin_addr) 
                + ":" + to_string(ntohs(from.sin_port)) + "\n";
            cout << message;
            found = true;
        }
    }

    closesocket(chS);
    return found;
}