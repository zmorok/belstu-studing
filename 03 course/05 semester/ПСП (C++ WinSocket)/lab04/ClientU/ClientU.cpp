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

    WSADATA wsdata;
    SOCKET cC;
    SOCKADDR_IN from;
    SOCKADDR_IN to;

    char* message = nullptr;
    
    try
    {
        
        if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0) throw SetErrorMsgText("WSAStartup:", WSAGetLastError());
        if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET) throw SetErrorMsgText("socket:", WSAGetLastError());
        
        to.sin_family = AF_INET;
        to.sin_port = htons(2000);
        to.sin_addr.s_addr = inet_addr("192.168.122.2");
        char bto[256];
        char bfrom[1000];
        int len = sizeof(to);
        int lenfrom = sizeof(from);

		cout << "Message to send: " << endl;

        string message;
		getline(cin, message);

        int send = sendto(cC, message.c_str(), message.length(), 0, (LPSOCKADDR)&to, len);
        int recv = recvfrom(cC, bfrom, sizeof(bfrom) - 1, 0, (LPSOCKADDR)&from, &lenfrom);
		bfrom[recv] = '\0';
        printf("%s: %s\n", "Package recieved", bfrom);
    }
    catch (string message)
    {
        cout << " ERROR: " << message << endl;
    }

    closesocket(cC);
    WSACleanup();
    system("pause");
}
