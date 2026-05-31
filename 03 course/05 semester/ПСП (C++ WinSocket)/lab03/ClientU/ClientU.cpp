#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <stdlib.h>

#include "SetErrorMsgText.h"
#include "SetupConsole.h"
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")

using namespace std;

int main()
{
	SetupConsoleLocale();
	WSADATA wsaData;

	try {
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) throw SetErrorMsgText("WSAStartup:", WSAGetLastError());

		SOCKET cS;
		if ((cS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET) throw SetErrorMsgText("клиентский сокет:", WSAGetLastError());
		
		// куда == сервер
		SOCKADDR_IN to;
		to.sin_family = AF_INET;
		to.sin_port = htons(2000);
		//string ip_addr; cout << "ip-адрес сервера: " << endl; getline(cin, ip_addr);
		//to.sin_addr.s_addr = inet_addr(ip_addr.c_str());
		to.sin_addr.s_addr = inet_addr("10.152.174.223"); // сервер
		//to.sin_addr.s_addr = inet_addr("10.152.174.130"); // клиентский
		string server = string(inet_ntoa(to.sin_addr)) + ':' + to_string(ntohs(to.sin_port));

		// откуда == куда == сервер
		SOCKADDR_IN from = to;
		int lenfrom = sizeof(from);
		
		cout << "кол-во передач: "; int n; cin >> n;
		string bto; cout << "сообщение: "; cin.ignore(); getline(cin, bto);

		cout << "частота (мс): "; int freq; cin >> freq;

		int ibuf = 0;
		char temp[1000];
		strcpy_s(temp, bto.c_str());

		cout << "ожидать ответ? (1 - да, 0 - нет): "; bool wait; cin >> wait;

		cout << "начало " << n << " циклов обмена с сервером..." << endl;
		cout << "============================" << endl;
		int cycle = 0;
		for (int i = 0; i < n; i++)
		{
			string msg = bto + to_string(++cycle);
			ibuf = 0;

			cout << "# sendto server... " << " (" + server + ")" << endl;
			if ((ibuf = sendto(cS, msg.c_str(), msg.size()+1, NULL, (sockaddr*)&to, sizeof(to))) == SOCKET_ERROR) throw SetErrorMsgText("sendto:", WSAGetLastError());
			else cout << "# sent: " << ibuf << " byte to server." << endl; ibuf = 0; memset(&temp, 0, sizeof(temp));

			if (wait) 
			{
				cout << "# recvfrom server..." << " (" + server + ")" << endl;
				if ((ibuf = recvfrom(cS, temp, sizeof(temp), NULL, (sockaddr*)&from, &lenfrom)) == SOCKET_ERROR) throw SetErrorMsgText("recvfrom:", WSAGetLastError());
				if (ibuf == 0) { cout << "соединение закрыто сервером"; break; }
				else cout << "# recived " << ibuf << " byte from server." << endl; ibuf = 0;

				cout << " - Сообщение от сервера: " << temp << endl;
			}

			Sleep(freq);
		}
	}
	catch (string err)
	{
		cout << "!! WSAGetLastError -- " + err << endl;
	}

	cout << "Клиент завершил работу. Нажмите любую клавишу, чтобы закрыть консоль..." << endl;
	system("pause");
	return 0;
}
