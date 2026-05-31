#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdlib.h>
#include <string>

#include "SetErrorMsgText.h"
#include "SetupConsole.h"
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")

using namespace std;

int main()
{
	SetupConsoleLocale();
	WSADATA wsaData;

	try
	{
		cout << "ServerU запуск..." << endl;
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 1) cout << "WSAStartup ошибка: " << WSAGetLastError() << endl;

		SOCKET sS;
		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET) throw SetErrorMsgText("серверный сокет: ", WSAGetLastError());
		cout << "сокет (sS) создан" << endl;

		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		string serv_ip_addr; cout << "ip-адрес сервера: " << endl; getline(cin, serv_ip_addr);
		serv.sin_addr.s_addr = inet_addr(serv_ip_addr.c_str());

		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR) throw SetErrorMsgText("ошибка бинд сокета (sS): ", WSAGetLastError());
		cout << "сервер запущен " << string(inet_ntoa(serv.sin_addr)) << ":" << to_string(ntohs(serv.sin_port)) << endl;

		char temp[1000];

		SOCKADDR_IN from;
		int ibuf = 0;
		int lenfrom = sizeof(from);

		srand((unsigned)time(nullptr));

		bool simulate_long_task = false;
		cout << "симуляция долгой задачи (от 0 до 500 мс)? (1 - да, 0 - нет): "; cin >> simulate_long_task;


		while (true)
		{
			memset(&temp, 0, sizeof(temp)); memset(&from, 0, sizeof(from)); ibuf = 0;


			cout << "# recvfrom..." << endl;
			string client;

			if ((ibuf = recvfrom(sS, temp, sizeof(temp), NULL, (sockaddr*)&from, &lenfrom)) == SOCKET_ERROR) throw SetErrorMsgText("recvfrom:", WSAGetLastError());
			else
			{
				client = string(inet_ntoa(from.sin_addr)) + ":" + to_string(ntohs(from.sin_port));
				cout << "# recived " << ibuf << " byte from " << client << endl; ibuf = 0;
			}

			

			string bfrom;
			bfrom = temp; memset(&temp, 0, sizeof(temp));
			cout << " - Сообщение: " << bfrom << endl;


			if (simulate_long_task)
			{
				int delay_ms = 2000; 
				Sleep(delay_ms);
				continue;
			}

			SOCKADDR_IN to = from;

			string bto = bfrom;
			strcpy_s(temp, bto.c_str());

			cout << "# sendto..." << endl;
			if ((ibuf = sendto(sS, temp, strlen(temp) + 1, NULL, (sockaddr*)&to, sizeof(to))) == SOCKET_ERROR) throw SetErrorMsgText("sendto:", WSAGetLastError());
			else cout << "# sent " << ibuf << " byte to " << client << endl;
		}

		if (closesocket(sS) == SOCKET_ERROR)	throw SetErrorMsgText("закрытие sS: ", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)		throw SetErrorMsgText("WSACleanup: ", WSAGetLastError());

		cout << "WSACleanup выполнен" << endl;

	}
	catch (string err)
	{
		cout << "!! WSAGetLastError -- " + err << endl;
	}

	cout << "Сервер завершил работу.Нажмите любую клавишу, чтобы закрыть консоль..." << endl;
	system("pause");
	return 0;
}