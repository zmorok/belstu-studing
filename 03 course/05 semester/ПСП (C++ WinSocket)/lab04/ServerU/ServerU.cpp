#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <Winsock2.h>
#pragma comment(lib, "WS2_32.lib")

#include "SetErrorMsgText.h"
#include "SetupConsole.h"

using namespace std;

int main()
{
	SetupConsoleLocale();

    WSAData wsdata;
    SOCKET sS{};
    SOCKADDR_IN serv;
    SOCKADDR_IN to;

    char name[] = "Hello";
    
    try
    {
        if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0) throw SetErrorMsgText("WSAStartup", WSAGetLastError());
        
        if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET) throw SetErrorMsgText("socket:", WSAGetLastError());
        
        serv.sin_family = AF_INET;
        serv.sin_port = htons(2000);
        serv.sin_addr.s_addr = inet_addr("192.168.122.2");
        if (bind(sS, LPSOCKADDR(&serv), sizeof(serv)) == SOCKET_ERROR) throw SetErrorMsgText("bind:", WSAGetLastError());
        
        while (true) {
            char bfrom[256];
            int len = sizeof(serv);
            int lento = sizeof(to);
            int recv = 0;
            if ((recv = recvfrom(sS, bfrom, sizeof(bfrom) - 1, NULL, (LPSOCKADDR)&to, &len)) == SOCKET_ERROR) throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
			bfrom[recv] = '\0';

			cout << "Package from: " << inet_ntoa(to.sin_addr) << ":" << ntohs(to.sin_port) << endl;
            cout << "### " << bfrom << endl;
            sendto(sS, bfrom, sizeof(bfrom) - 1, NULL, (LPSOCKADDR)&to, lento);
        }
    }
    catch (string message)
    {
        cout << " ERROR: " << message << endl;
    }

    closesocket(sS);
    WSACleanup();
    system("pause");
}