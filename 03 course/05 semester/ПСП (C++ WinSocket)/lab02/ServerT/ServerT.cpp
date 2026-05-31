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
	// настройка консоли и дата вод виндовс сокет
	SetupConsoleLocale();
	WSADATA wsaData;
	
	try
	{
		// инициализация виндовс сокет
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 1) cout << "WSAStartup ошибка: " << WSAGetLastError() << endl;
		
		// создание сокета сервера
		SOCKET sS;
		if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET) throw SetErrorMsgText("серверный сокет: ", WSAGetLastError());
		cout << "сокет (sS) создан" << endl;


		// заполнение структуры сокета сервера == точка подключения клиентов
		// адрес, порт и протокол
		// ===========================================
		string serv_ip_addr;
		cout << "ip-адрес сервера: " << endl;
		getline(cin, serv_ip_addr);

		
		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		serv.sin_addr.s_addr = inet_addr(serv_ip_addr.c_str());
		// ===========================================

		// связывание сокета с адресом и прослушка порта
		// ===========================================
		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR) throw SetErrorMsgText("ошибка бинд сокета (sS): ", WSAGetLastError());
		cout << "сервер запущен " << string(inet_ntoa(serv.sin_addr)) << ":" << to_string(ntohs(serv.sin_port)) << endl;

		if (listen(sS, SOMAXCONN) == SOCKET_ERROR) throw SetErrorMsgText("ошибка прослушки (sS): ", WSAGetLastError());
		// ===========================================

		// сокет для общения с клиентом
		SOCKET cS;

		// приём клиентов
		// цикл while (true) для последовательного обслуживания клиентов
		while (true)
		{
			// структура для хранения адреса текущего клиента
			SOCKADDR_IN clnt;
			int lclnt = sizeof(clnt);

			// ожидание клиента и получение его сокета
			cout << "ждём клиента..." << endl;
			if ((cS = accept(sS, (LPSOCKADDR)&clnt, &lclnt)) == INVALID_SOCKET) { SetErrorMsgText("accept error: ", WSAGetLastError()); continue; }

			// условная строка клиента
			string client = string(inet_ntoa(clnt.sin_addr)) + ":" + to_string(ntohs(clnt.sin_port));

			cout << client << " подключился..." << endl;

			// буферы для приёма и передачи, а также счётчики принятых и переданных байт
			// обработка n передач от клиента
			char ibuf[100], obuf[100];
			int countibuf = 0, countobuf = 0;
			for (;;)
			{
				// сессия с клиентом

				// приём
				if ((countibuf = recv(cS, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR) { SetErrorMsgText("recv (cS): ", WSAGetLastError()); break; }
				// если сообщение нулевой длины, клиент отключился
				if (countibuf == 0) { cout << client << " отключился" << endl; break; }

				// отправка обратно клиенту его сообщения
				if (send(cS, ibuf, strlen(ibuf) + 1, NULL) == SOCKET_ERROR) { SetErrorMsgText("send (cS): ", WSAGetLastError()); break; }
				else cout << "# " << client << " отправлено: \"" << ibuf << "\"\n";
			}
		}
		
		if (closesocket(sS) == SOCKET_ERROR)	throw SetErrorMsgText("закрытие sS: ", WSAGetLastError());
		if (closesocket(cS) == SOCKET_ERROR)	throw SetErrorMsgText("закрытие cS: ", WSAGetLastError());
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